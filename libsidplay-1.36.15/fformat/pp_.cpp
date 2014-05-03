//
// /home/ms/sidplay/libsidplay/fformat/RCS/pp_.cpp,v
//
// For more info get ``depp.cpp'' and ``ppcl.cpp''.

#include "pp_.h"


const char PP_ID[] = "PP20";

const udword PP_BITS_FAST = 0x09090909;
const udword PP_BITS_MEDIOCRE = 0x090a0a0a;
const udword PP_BITS_GOOD = 0x090a0b0b;
const udword PP_BITS_VERYGOOD = 0x090a0c0c;
const udword PP_BITS_BEST = 0x090a0c0d;

const char text_packeddatacorrupt[]	= "PowerPacker: Packed data is corrupt";
const char text_unrecognized[]	= "PowerPacker: Unrecognized compression method";
const char text_unknownformat[] = "Not compressed with PowerPacker";
const char text_notenoughmemory[] = "Not enough free memory";
const char text_fast[] = "PowerPacker: fast compression";
const char text_mediocre[] = "PowerPacker: mediocre compression";
const char text_good[] = "PowerPacker: good compression";
const char text_verygood[] = "PowerPacker: very good compression";
const char text_best[] = "PowerPacker: best compression";

const char* ppErrorString;

extern udword ppUncompressedLen(); // return the length of the uncompressed data

static void ppfreemem();
inline udword ppread( int );
inline void ppbytes();
inline void ppsequence();
inline void bytestodword();

static ubyte* sourcebuf;
static ubyte* readptr;
static ubyte* writeptr;
static ubyte* startptr;
static udword current;       // compressed data longword
static int bits;             // number of bits in 'current' to evaluate
static ubyte efficiency[4];
static udword outputlen;
static char globalerror;


// returns: FALSE = file is not in (supported) PowerPacker format
//          TRUE = successful return           
//          (-1) = error during decompression

int depp( ifstream& source, ubyte** destRef )
{
	ppErrorString = text_unknownformat;
	globalerror = FALSE;
	outputlen = 0;
  
	// Check for header signature
	source.seekg(0,ios::beg);
	char sig[5];
	source.read(sig,4);
	sig[4] = 0;
	if ( strcmp(sig,PP_ID) != 0 )
	{
		return FALSE;
	}
	
	// Read in efficiency table.
	source.read(efficiency,4);
	udword eff = readEndian(efficiency[0],efficiency[1],efficiency[2],efficiency[3]);
	if (( eff != PP_BITS_FAST ) &&
		( eff != PP_BITS_MEDIOCRE ) &&
		( eff != PP_BITS_GOOD ) &&
		( eff != PP_BITS_VERYGOOD ) &&
		( eff != PP_BITS_BEST ))
	{
		ppErrorString = text_unrecognized;
		return (-1);
	}

  // move file pointer to end of file
  source.seekg( 0, ios::end );
#if defined(__POWERPC__)
  udword inputlen = (source.seekg(0,ios::end)).offset();
#else
  udword inputlen = (udword)source.tellg();
#endif
  source.seekg( 0, ios::beg );

  // allocate memory for input file
#if defined(_Windows) && !defined(__WIN32__)
  if (( sourcebuf = (ubyte *)GlobalAllocPtr ( GPTR, inputlen )) == 0 )  {
	ppErrorString = text_notenoughmemory;
	return(-1);
  }
#else
  if (( sourcebuf = new ubyte[inputlen]) == 0 )  {
	ppErrorString = text_notenoughmemory;
	return(-1);
  }
#endif

#if defined(__BORLANDC__) && !defined(__WIN32__)
  // for 16-bit segmented Windows we would have to change this
  // to be able to load beyond the 64KB segment boundaries
#endif
  udword restfilelen = inputlen;
  while ( restfilelen > INT_MAX )  {
	source.read( (ubyte*)sourcebuf + (inputlen - restfilelen), INT_MAX );
	restfilelen -= INT_MAX;
  }
  if ( restfilelen > 0 )
  source.read( (ubyte*)sourcebuf + (inputlen - restfilelen), restfilelen );

  // reset file pointer
  source.seekg( 0, ios::beg );

  // backwards decompression
  readptr = sourcebuf + inputlen -4;

  // uncompressed length in bits 31-8 of last dword
  outputlen = readEndian(0,*readptr,*(readptr+1),*(readptr+2));

  // Free any previously existing destination buffer.
  if ( *destRef != 0 )
  {
	delete[] *destRef;
  }
  // Allocate memory for output data.
#if defined(_Windows) && !defined(__WIN32__)
  if (( *destRef = (ubyte *)GlobalAllocPtr ( GPTR, outputlen )) == 0 )
  {
	ppErrorString = text_notenoughmemory;
	return(-1);
  }
#else
  if (( *destRef = new ubyte[outputlen]) == 0 )
  {
	ppErrorString = text_notenoughmemory;
	return(-1);
  }
#endif
  
  switch ( eff)  {
   case PP_BITS_FAST:
	ppErrorString = text_fast;
	break;
   case PP_BITS_MEDIOCRE:
	ppErrorString = text_mediocre;
	break;
   case PP_BITS_GOOD:
	ppErrorString = text_good;
	break;
   case PP_BITS_VERYGOOD:
	ppErrorString = text_verygood;
	break;
   case PP_BITS_BEST:
	ppErrorString = text_best;
	break;
  }
  
  // put destptr to end of uncompressed data
  writeptr = *destRef + outputlen;
  // lowest dest. address for range-checks
  startptr = *destRef;

  // read number of unused bits in 1st data dword
  // from lowest bits 7-0 of last dword
  bits = 32 - *(sourcebuf + inputlen -1);

  // decompress data
  
  bytestodword();

  if ( bits != 32 )
	current = ( current >> ( 32 - bits ));

  do  {
	if ( ppread( 1) == 0 )  
	  ppbytes();
	if ( writeptr > *destRef )  
	  ppsequence();
	if ( globalerror )  {
	  ppfreemem();
	  return(-1);
	}
  } while ( writeptr > *destRef );

  ppfreemem();
  return TRUE;
}


void ppfreemem()  {
#if defined(_Windows) && !defined(__WIN32__)
  if ( sourcebuf != 0 )
	GlobalFreePtr ( sourcebuf );
  sourcebuf = 0;
#else
  if ( sourcebuf != 0 )
	delete[] sourcebuf;
  sourcebuf = 0;
#endif  
}


udword ppUncompressedLen()  {
  return(outputlen);
}

//
inline udword ppread( int count )  {
  udword data = 0;
  // read 'count' bits of packed data
  for ( ; count > 0; count-- )  {
	// equal to shift left
	data += data;
	// merge bit 0
    data = ( data | (current & 1) );
    current = ( current >> 1 );
    if ( (--bits) == 0 )  {
      bytestodword();
      bits = 32;
    }
  }
  return data;
}

//
inline void ppbytes()  {
  udword count, add;
  count = ( add = ppread( 2 ) );
  while ( add == 3 )  {
	add = ppread( 2);
	count += add;
  }
  count++;
  if ( writeptr > startptr )  {
	for ( ; count > 0 ; count-- )  
	  *(--writeptr) = (ubyte)ppread( 8 );
  }
}

//
inline void ppsequence()  {
  udword offset, length, add;
  int offsetbitlen;
  length = ppread( 2 );       // length -2
  offsetbitlen = (int)efficiency[length];
  length += 2;
  if ( length != 5 )
	offset = ppread( offsetbitlen );
  else  {
	if ( ppread( 1 ) == 0 )
	  offsetbitlen = 7;
	offset = ppread( offsetbitlen );
	add = ppread( 3 );
	length += add;
	while ( add == 7 )  {
	  add = ppread(3);
	  length += add;
	}
  }
  if ( writeptr > startptr )  {
	for ( ; length > 0 ; length-- )   {
	  --writeptr;
	  *writeptr = *(writeptr + 1 + offset);
	}
  }
}

// moves four bytes to Motorola big-endian double-word
inline void bytestodword()  {
  readptr -= 4;
  if ( readptr < sourcebuf )  {
	ppErrorString = text_packeddatacorrupt;
	globalerror = TRUE;
  }
  else 
	current = readEndian(*readptr,*(readptr+1),*(readptr+2),*(readptr+3));
}
