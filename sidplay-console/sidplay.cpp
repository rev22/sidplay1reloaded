//
// /home/ms/sidplay/RCS/sidplay.cpp,v
//

#include <iostream.h>
#include <iomanip.h>
#include <fstream.h>
#include <stdlib.h>
#include <unistd.h>

#include "player.h"
#include "myendian.h"
#include "audiodrv.h"

#if defined(__amigaos__)
#define EXIT_ERROR_STATUS (20)
#else
#define EXIT_ERROR_STATUS (-1)
#endif

// Error and status message numbers.
enum
{
	ERR_NOT_ENOUGH_MEMORY,
	ERR_SYNTAX,
	ERR_ENDIANESS
};

void error( char*, char* );  // print out a text in "ERROR: string1 'string2'" format.
void printtext( int messageNum );

static bool verboseOutput = false;

int main(int argc, char *argv[])
{
	cout 
		<< endl
		<< "SIDPLAY   Music player and C64 SID chip emulator   Version " << emu_version << endl
		<< "Copyright (c) 1994-1997 Michael Schwendt   All rights reserved." << endl
#if defined(sgi)
		<< "Ported to SGI by <aagero@aage.priv.no>" << endl
#elif defined(hpux)
	    << "Ported to HP-UX by <esap@cs.tut.fi>" << endl
#elif defined(__amigaos__)
	    << "Ported to AmigaOS by <phillwooller@geocities.com>" << endl
#endif	
		<< endl;
	
	// ======================================================================
	// INITIALIZE THE EMULATOR ENGINE
	// ======================================================================

	// Initialize the SID-Emulator Engine to defaults.
	emuEngine myEmuEngine;
	// Everything went okay ?
	if ( !myEmuEngine )
	{
		// So far the only possible error.
		printtext(ERR_NOT_ENOUGH_MEMORY);
	}
	if ( !myEmuEngine.verifyEndianess() )
	{
		printtext(ERR_ENDIANESS);
	}

	// Get the default configuration.
	struct emuConfig myEmuConfig;
	myEmuEngine.getConfig(myEmuConfig);

	// ======================================================================

	if ( argc < 2 )             // at least one argument required
		printtext(ERR_SYNTAX);

	uword selectedSong = 0;
	
	// Default audio settings.
	myEmuConfig.frequency = 22050;
	myEmuConfig.channels = SIDEMU_MONO;
	myEmuConfig.bitsPerSample = SIDEMU_8BIT;
	uword fragments = 16;
	uword fragSizeBase = 12;
	int forceBufSize = 0;  // get it from argument line
    
	ubyte infile = 0;
	
	// parse command line arguments
	int a = 1;
	while ((a < argc) && (argv[a] != NULL))  
	{
		if ( argv[a][0] == '-')  
		{
			// reading from stdin
			if ( strlen(argv[a]) == 1 ) 
				if ( infile == 0 )
			{
				infile = a;
				break;
			}
			else
				printtext(ERR_SYNTAX);
			switch ( argv[a][1] )
			{
#if defined(linux) || defined(__FreeBSD__) || defined(__amigaos__)
			 case '1':
				if ( argv[a][2] == '6' )
					myEmuConfig.bitsPerSample = SIDEMU_16BIT;
				break;
#endif		
			 case 'c':
				myEmuConfig.forceSongSpeed = true;
				break;
			 case 'a':
				if ( argv[a][2] == '2' )
					myEmuConfig.memoryMode = MPU_BANK_SWITCHING;
				else
					myEmuConfig.memoryMode = MPU_PLAYSID_ENVIRONMENT;
				break;
			 case 'b':
				if ( argv[a][2] == 'n' )  
				{
					fragments = (unsigned)atoi(argv[a]+3);
					if (( fragments < 2 ) || ( fragments > 255 )) 
						fragments = 2;
				}
				else if ( argv[a][2] == 's' )  
				{
					fragSizeBase = (unsigned)atoi(argv[a]+3);
					if (( fragSizeBase < 7 ) || ( fragSizeBase > 17 )) 
						fragSizeBase = 14;
				}
				else
				{
					forceBufSize = atoi(argv[a]+2);
				}
				break;
			 case 'f':
				myEmuConfig.frequency = (udword)atol(argv[a]+2);
				break;
			 case 'h':
				printtext(ERR_SYNTAX);
				break;
			 case 'n':
				if ( argv[a][2] == 'f' )  
					myEmuConfig.emulateFilter = false;
				else
					myEmuConfig.clockSpeed = SIDTUNE_CLOCK_NTSC;
				break;
			 case 'o':
				selectedSong = atoi(argv[a]+2);
				break;
			 case 'p':
				if ( argv[a][2] == 'c' )  
				{
					myEmuConfig.autoPanning = SIDEMU_CENTEREDAUTOPANNING;
				}
				break;
			 case 's':
				myEmuConfig.channels = SIDEMU_STEREO;
				if ( argv[a][2] == 's' )  
				{
					myEmuConfig.volumeControl = SIDEMU_STEREOSURROUND;
				}
				break;
			 case 'v':
				verboseOutput = true;
				break;
			 default:
				printtext(ERR_SYNTAX);
				break;
			}
		}
		else  
		{
			if ( infile == 0 )
				infile = a;  // filename argument
			else
				printtext(ERR_SYNTAX);
		}
		a++;  // next argument
	};
	
	if (infile == 0)
	{
		printtext(ERR_SYNTAX);
	}

	// ======================================================================
	// VALIDATE SID EMULATOR SETTINGS
	// ======================================================================
	
	if ((myEmuConfig.autoPanning!=SIDEMU_NONE) && (myEmuConfig.channels==SIDEMU_MONO))
	{
		myEmuConfig.channels = SIDEMU_STEREO;  // sane
	}
	if ((myEmuConfig.autoPanning!=SIDEMU_NONE) && (myEmuConfig.volumeControl==SIDEMU_NONE))
	{
		myEmuConfig.volumeControl = SIDEMU_FULLPANNING;  // working
	}

	// ======================================================================
	// INSTANTIATE A SIDTUNE OBJECT
	// ======================================================================

	sidTune myTune( argv[infile] );
	struct sidTuneInfo mySidInfo;
	myTune.getInfo( mySidInfo );
	if ( !myTune )  
	{
		cerr << "SIDPLAY: " << mySidInfo.statusString << endl;
		exit(EXIT_ERROR_STATUS);
	}
	else
	{
		if (verboseOutput)
		{
			cout << "File format  : " << mySidInfo.formatString << endl;
			cout << "Filenames    : " << mySidInfo.dataFileName << ", "
				<< mySidInfo.infoFileName << endl;
			cout << "Condition    : " << mySidInfo.statusString << endl;
		}
		cout << "--------------------------------------------------" << endl;
		if ( mySidInfo.numberOfInfoStrings == 3 )
		{
			cout << "Name         : " << mySidInfo.nameString << endl;
			cout << "Author       : " << mySidInfo.authorString << endl;
			cout << "Copyright    : " << mySidInfo.copyrightString << endl;
		}
		else
		{
			for ( int infoi = 0; infoi < mySidInfo.numberOfInfoStrings; infoi++ )
				cout << "Description  : " << mySidInfo.infoString[infoi] << endl;
		}
		cout << "--------------------------------------------------" << endl;
		if (verboseOutput)
		{
			cout << "Load address : $" << hex << setw(4) << setfill('0') 
				<< mySidInfo.loadAddr << endl;
			cout << "Init address : $" << hex << setw(4) << setfill('0') 
				<< mySidInfo.initAddr << endl;
			cout << "Play address : $" << hex << setw(4) << setfill('0') 
				<< mySidInfo.playAddr << dec << endl;
		}
	}

	// ======================================================================
	// CONFIGURE THE AUDIO DRIVER
	// ======================================================================

	// Instantiate the audio driver. The capabilities of the audio driver
	// can override the settings of the SID emulator.
	audioDriver myAudio;
	if ( !myAudio.IsThere() )
	{
		cerr << "SIDPLAY: No audio device available !" << endl;
		exit(EXIT_ERROR_STATUS);
	}
	// Open() does not accept the "bitsize" value on all platforms, e.g.
	// Sparcstations 5 and 10 tend to be 16-bit only at rates above 8000 Hz.
	if ( !myAudio.Open(myEmuConfig.frequency,myEmuConfig.bitsPerSample,
					   myEmuConfig.channels,fragments,fragSizeBase))
	{
		cerr << "SIDPLAY: " << myAudio.GetErrorString() << endl;
		exit(EXIT_ERROR_STATUS);
	}
	if (verboseOutput)
	{
		cout << "Block size   : " << (udword)myAudio.GetBlockSize() << endl
			<< "Fragments    : " << myAudio.GetFragments() << endl;
	}
	
	// ======================================================================
	// CONFIGURE THE EMULATOR ENGINE
	// ======================================================================

	// Configure the SID emulator according to the audio driver settings.
	myEmuConfig.frequency = myAudio.GetFrequency();
	myEmuConfig.bitsPerSample = myAudio.GetSamplePrecision();
	myEmuConfig.sampleFormat = myAudio.GetSampleEncoding();
	myEmuEngine.setConfig( myEmuConfig );
    // Print the relevant settings.
	if (verboseOutput)
	{
		cout << "Frequency    : " << dec << myEmuConfig.frequency << " Hz" << endl;
		cout << "SID Filter   : " << ((myEmuConfig.emulateFilter == true) ? "Yes" : "No") << endl;
		if (myEmuConfig.memoryMode == MPU_PLAYSID_ENVIRONMENT)
		{
			cout << "Memory mode  : PlaySID (this is supposed to fix PlaySID-specific rips)" << endl;
		}
		else if (myEmuConfig.memoryMode == MPU_TRANSPARENT_ROM)
		{
			cout << "Memory mode  : Transparent ROM (SIDPLAY default)" << endl;
		}
		else if (myEmuConfig.memoryMode == MPU_BANK_SWITCHING)
		{
			cout << "Memory mode  : Bank Switching" << endl;
		}
	}
	
	// ======================================================================
	// INITIALIZE THE EMULATOR ENGINE TO PREPARE PLAYING A SIDTUNE
	// ======================================================================

	if ( !sidEmuInitializeSong(myEmuEngine,myTune,selectedSong) )
	{
		cerr << "SIDPLAY: SID Emulator Engine components not ready" << endl;
		exit(EXIT_ERROR_STATUS);
	}
	// Read out the current settings of the sidtune.
	myTune.getInfo( mySidInfo );
	if ( !myTune )
	{
		cerr << "SIDPLAY: " << mySidInfo.statusString << endl;
		exit(EXIT_ERROR_STATUS);
	}
	cout << "Setting song : " << mySidInfo.currentSong
		<< " out of " << mySidInfo.songs
		<< " (default = " << mySidInfo.startSong << ')' << endl;
	if (verboseOutput)
	{
		cout << "Song speed   : " << mySidInfo.speedString << endl;
	}
	
	// ======================================================================
	// KEEP UP A CONTINUOUS OUTPUT SAMPLE STREAM
	// ======================================================================

	int bufSize = myAudio.GetBlockSize();
	if (forceBufSize != 0)
	{
		bufSize = forceBufSize;
		if (verboseOutput)
		{
			cout << "Buffer size  : " << bufSize << endl;
		}
	}
	ubyte* buffer;
	if ((buffer = new ubyte[bufSize]) == 0)
    {
		printtext(ERR_NOT_ENOUGH_MEMORY);
	    exit(EXIT_ERROR_STATUS);
    }
	cout << "Playing, press ^C to stop ..." << endl;
	for (;;)
	{
		sidEmuFillBuffer(myEmuEngine,myTune,buffer,bufSize);
		myAudio.Play(buffer,bufSize);
	}

	// Will possibly never come this far.
	exit(0);
}


void error(char* s1, char* s2 = "")
{
	cerr << "SIDPLAY: ERROR: " << s1 << ' ' << "'" << s2 << "'" << endl;
	exit(EXIT_ERROR_STATUS);
}


void printtext(int number)
{
  switch (number)  
	{
	 case ERR_ENDIANESS:
		{
			cerr << "SIDPLAY: ERROR: Hardware endianess improperly configured." << endl;
			exit(EXIT_ERROR_STATUS);
			break;
		}
	 case ERR_NOT_ENOUGH_MEMORY:
		cerr << "SIDPLAY: ERROR: Not enough memory" << endl;
		exit(EXIT_ERROR_STATUS);
	 case ERR_SYNTAX:
		cout << " syntax: sidplay [-<command>] <datafile>|-" << endl
			<< " commands: -h       display this screen" << endl
			<< "           -v       verbose output" << endl
			<< "           -f<num>  set frequency in Hz (default: 22050)" << endl
			<< "           -o<num>  set song number (default: preset)" << endl
			<< "           -a       improve PlaySID compatibility (read the docs !)" << endl
			<< "           -a2      bank switching mode (overrides -a)" << endl
#if defined(linux) || defined(__FreeBSD__) || defined(__amigaos__)
			<< "           -16      enable 16-bit sample mixing" << endl
#endif	  
#if defined(linux) || defined(__FreeBSD__) || defined(__amigaos__) || defined(hpux)
			<< "           -s       enable stereo replay" << endl
			<< "           -ss      enable stereo surround" << endl
			<< "           -pc      enable centered auto-panning (stereo only)" << endl
#endif
			<< "           -nf      no SID filter emulation" << endl
			<< "           -n       set NTSC clock speed (default: PAL)" << endl
			<< "           -c       force song speed = clock speed (PAL/NTSC)" << endl
			<< "           -bn<num> set number of audio buffer fragments to use" << endl
			<< "           -bs<num> set size 2^<num> of audio buffer fragments" << endl
			<< "           -b<num>  set sample buffer size" << endl
			<< endl;
		exit(EXIT_ERROR_STATUS);
	 default:
		cerr << "SIDPLAY: ERROR: Internal system error" << endl;
		exit(EXIT_ERROR_STATUS);
	}
}
