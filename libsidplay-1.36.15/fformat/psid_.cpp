//
// 1997/05/11 11:29:06
//

#include "psid_.h"


const char text_format[] = "PlaySID one-file format (PSID)";
const char text_psidTruncated[] = "ERROR: PSID file is most likely truncated";

struct psidHeader
{
	//
	// All values in big-endian order.
	//
	char id[4];          // 'PSID'
	ubyte version[2];    // 0x0001 or 0x0002
	ubyte data[2];       // 16-bit offset to binary data in file
	ubyte load[2];       // 16-bit C64 address to load file to
	ubyte init[2];       // 16-bit C64 address of init subroutine
	ubyte play[2];       // 16-bit C64 address of play subroutine
	ubyte songs[2];      // number of songs
	ubyte start[2];      // start song (1-256 !)
	ubyte speed[4];      // 32-bit speed info
		                 // bit: 0=50 Hz, 1=CIA 1 Timer A (default: 60 Hz)
	char name[32];       // ASCII strings, 31 characters long and
	char author[32];     // terminated by a trailing zero
	char copyright[32];  //
	ubyte flags[2];      // only version 0x0002
	ubyte reserved[4];   // only version 0x0002
};


bool sidTune::PSID_fileSupport( void* buffer, udword bufLen )
{
	// Remove any format description or format error string.
	info.formatString = 0;

	// Require a first minimum size.
	if (bufLen < 6)
	{
		return false;
	}
	// Now it is safe to access the first bytes.
	// Require a valid ID and version number.
	psidHeader* pHeader = (psidHeader*)buffer;
	if ( (readBEdword((ubyte*)pHeader->id) != 0x50534944)  // "PSID" ?
		|| (readBEword(pHeader->version) >= 3) )
	{
		return false;
	}
	// Due to security concerns, input must be at least as long as version 1
	// plus C64 load address data. That is the area which will be accessed.
	if ( bufLen < (sizeof(psidHeader)+2) )
	{
		info.formatString = text_psidTruncated;
		return false;
	}

	fileOffset = readBEword(pHeader->data);
	info.loadAddr = readBEword(pHeader->load);
	info.initAddr = readBEword(pHeader->init);
	info.playAddr = readBEword(pHeader->play);
	info.songs = readBEword(pHeader->songs);
	info.startSong = readBEword(pHeader->start);

	if (info.songs > classMaxSongs)
	{
		info.songs = classMaxSongs;
	}
	
	// Create the speed/clock setting table.
	udword oldStyleSpeed = readBEdword(pHeader->speed);
	convertOldStyleSpeedToTables(oldStyleSpeed);
	
	info.musPlayer = false;
	if ( readBEword(pHeader->version) >= 2 )
	{
		if (( readBEword(pHeader->flags) & 1 ) == 1 )
		{
			info.musPlayer = true;
		}
	}
  
	if ( info.loadAddr == 0 )
	{
		ubyte* pData = (ubyte*)buffer + fileOffset;
		info.loadAddr = readEndian( *(pData+1), *pData );
		fileOffset += 2;
	}
	if ( info.initAddr == 0 )
	{
		info.initAddr = info.loadAddr;
	}
	if ( info.startSong == 0 ) 
	{
		info.startSong = 1;
	}
	
	// Now adjust MUS songs.
	if ( info.musPlayer )
	{
		info.loadAddr = 0x1000;
		info.initAddr = 0xc7b0;
		info.playAddr = 0;
	}

	// Correctly terminate the info strings.
	pHeader->name[31] = 0;
	pHeader->author[31] = 0;
	pHeader->copyright[31] = 0;

	// Copy info strings, so they will not get lost.
	strcpy( &infoString[0][0], pHeader->name );
	info.nameString = &infoString[0][0];
	info.infoString[0] = &infoString[0][0];
	strcpy( &infoString[1][0], pHeader->author );
	info.authorString = &infoString[1][0];
	info.infoString[1] = &infoString[1][0];
	strcpy( &infoString[2][0], pHeader->copyright );
	info.copyrightString = &infoString[2][0];
	info.infoString[2] = &infoString[2][0];
	info.numberOfInfoStrings = 3;
	
	info.formatString = text_format;
	return true;
}


bool sidTune::PSID_fileSupportSave( ofstream& fMyOut, ubyte* dataBuffer )
{
	psidHeader myHeader;
	writeBEdword((ubyte*)myHeader.id,0x50534944);  // 'PSID'
	writeBEword(myHeader.version,2);
	writeBEword(myHeader.data,0x7C);
	writeBEword(myHeader.load,0);
	writeBEword(myHeader.init,info.initAddr);
	writeBEword(myHeader.play,info.playAddr);
	writeBEword(myHeader.songs,info.songs);
	writeBEword(myHeader.start,info.startSong);

	udword speed = 0;
	int maxBugSongs = ((info.songs <= 32) ? info.songs : 32);
	for (int s = 0; s < maxBugSongs; s++)
	{
		if (songSpeed[s] == SIDTUNE_SPEED_CIA_1A)
		{
			speed |= (1<<s);
		}
	}
	writeBEdword(myHeader.speed,speed);

	uword tmpFlags = 0;
	if ( info.musPlayer )
	{
		tmpFlags |= 1;
	}
	writeBEword(myHeader.flags,tmpFlags);
	writeBEdword(myHeader.reserved,0);
	for ( int i = 0; i < 32; i++ )
	{
		myHeader.name[i] = 0;
		myHeader.author[i] = 0;
		myHeader.copyright[i] = 0;
	}
	strncpy( myHeader.name, info.nameString, 31 );
	strncpy( myHeader.author, info.authorString, 31 );
	strncpy( myHeader.copyright, info.copyrightString, 31 );
	fMyOut.write( (char*)&myHeader, sizeof(psidHeader) );
	
	// Save C64 lo/hi load address (little-endian).
	ubyte saveAddr[2];
	saveAddr[0] = info.loadAddr & 255;
	saveAddr[1] = info.loadAddr >> 8;
	fMyOut.write( saveAddr, 2 );
	// Data starts at: bufferaddr + fileoffset
	// Data length: datafilelen - fileoffset
	fMyOut.write( dataBuffer + fileOffset, info.dataFileLen - fileOffset );
	if ( !fMyOut )
	{
		return false;
	}
	else
	{
		return true;
	}
}
