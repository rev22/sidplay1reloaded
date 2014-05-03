//
// /home/ms/source/sidplay/RCS/sidplay.cpp,v
//

#include <iostream.h>
#include <iomanip.h>
#include <fstream.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "player.h"
#include "myendian.h"
#include "fformat.h"
#include "audiodrv.h"

#if defined(__amigaos__)
#define EXIT_ERROR_STATUS (20)
#else
#define EXIT_ERROR_STATUS (-1)
#endif

#if defined(HAVE_SGI)
#define DISALLOW_16BIT_SOUND
#define DISALLOW_STEREO_SOUND
#endif

// Error and status message numbers.
enum
{
    ERR_SYNTAX,
	ERR_NOT_ENOUGH_MEMORY,
    ERR_SIGHANDLER
};

void printError(char* arg0, int messageNum);
void printSyntax(char* arg0);

bool exitFlag;

void (*oldSigHupHandler)(int);
void (*oldSigIntHandler)(int);
void (*oldSigQuitHandler)(int);
void (*oldSigTermHandler)(int);
void mysighandler(int signum)
{
    switch (signum)
    {
     case SIGHUP:
        {
            exitFlag = true;
            break;
        }
     case SIGINT:
        {
            exitFlag = true;
            break;
        }
     case SIGQUIT:
        {
            exitFlag = true;
            break;
        }
     case SIGTERM:
        {
            exitFlag = true;
            break;
        }
     default:
        break;
    }
}

static bool verboseOutput = false;

int main(int argc, char *argv[])
{
	// ======================================================================
	// INITIALIZE THE EMULATOR ENGINE
	// ======================================================================

	// Initialize the SID-Emulator Engine to defaults.
	emuEngine myEmuEngine;
	// Everything went okay ?
	if ( !myEmuEngine )
	{
		// So far the only possible error.
		printError(argv[0],ERR_NOT_ENOUGH_MEMORY);
        exit(EXIT_ERROR_STATUS);
	}
	if ( !myEmuEngine.verifyEndianess() )
	{
        cerr << argv[0] << ": Hardware endianess improperly configured during compilation." << endl;
        exit(EXIT_ERROR_STATUS);
	}

	// Get the default configuration.
	struct emuConfig myEmuConfig;
	myEmuEngine.getConfig(myEmuConfig);

	// ======================================================================

	if ( argc < 2 )  // at least one argument required
    {
        printError(argv[0],ERR_SYNTAX);
        exit(EXIT_ERROR_STATUS);
    }

    cout
        << "SIDPLAY   Music player and C64 SID chip emulator   "
        << "Version " << myEmuEngine.getVersionString() << endl;
        
	uword selectedSong = 0;
	
	// Default audio settings.
	myEmuConfig.frequency = 22050;
	myEmuConfig.channels = SIDEMU_MONO;
	myEmuConfig.bitsPerSample = SIDEMU_8BIT;
	uword fragments = 16;
	uword fragSizeBase = 12;
	int forceBufSize = 0;  // get it from argument line
    
	int infile = 0;
	
	// parse command line arguments
	int a = 1;
	while ((a < argc) && (argv[a] != NULL))  
	{
		if ( argv[a][0] == '-')  
		{
			// Reading from stdin?
			if ( strlen(argv[a]) == 1 ) 
				if ( infile == 0 )
			{
				infile = a;
				break;
			}
			else
            {
                printError(argv[0],ERR_SYNTAX);
                exit(EXIT_ERROR_STATUS);
            }
            if (strnicmp(argv[a],"--help",6) == 0)
            {
                printSyntax(argv[0]);
                exit(0);
            }
			switch ( argv[a][1] )
			{
#if !defined(DISALLOW_16BIT_SOUND)
			 case '1':
				if ( argv[a][2] == '6' )
					myEmuConfig.bitsPerSample = SIDEMU_16BIT;
				break;
#endif		
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
			 case 'c':
				myEmuConfig.forceSongSpeed = true;
				break;
			 case 'f':
				myEmuConfig.frequency = (udword)atol(argv[a]+2);
				break;
			 case 'h':
				printSyntax(argv[0]);
                exit(0);
			 case 'n':
				if ( argv[a][2] == 'f' )  
					myEmuConfig.emulateFilter = false;
				else if ( argv[a][2] == 's' )
					myEmuConfig.mos8580 = true;
				else
					myEmuConfig.clockSpeed = SIDTUNE_CLOCK_NTSC;
				break;
			 case 'o':
				selectedSong = atoi(argv[a]+2);
				break;
#if !defined(DISALLOW_STEREO_SOUND)
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
#endif
			 case 'v':
				verboseOutput = true;
				break;
			 default:
				printSyntax(argv[0]);
				exit(0);
			}
		}
		else  
		{
			if ( infile == 0 )
				infile = a;  // filename argument
			else
            {
				printSyntax(argv[0]);
				exit(0);
            }
		}
		a++;  // next argument
	};
	
	if (infile == 0)
	{
        // Neither file nor stdin.
		printSyntax(argv[0]);
        exit(0);
	}

	// ======================================================================
	// VALIDATE SID EMULATOR SETTINGS
	// ======================================================================
	
	if ((myEmuConfig.autoPanning!=SIDEMU_NONE)
        && (myEmuConfig.channels==SIDEMU_MONO))
	{
		myEmuConfig.channels = SIDEMU_STEREO;  // sane
	}
	if ((myEmuConfig.autoPanning!=SIDEMU_NONE)
        && (myEmuConfig.volumeControl==SIDEMU_NONE))
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
		cerr << argv[0] << ": " << mySidInfo.statusString << endl;
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
		cerr << argv[0] << ": No audio device available!" << endl;
		exit(EXIT_ERROR_STATUS);
	}
	// Open() does not accept the "bitsize" value on all platforms, e.g.
	// Sparcstations 5 and 10 tend to be 16-bit only at rates above 8000 Hz.
	if ( !myAudio.Open(myEmuConfig.frequency,myEmuConfig.bitsPerSample,
					   myEmuConfig.channels,fragments,fragSizeBase))
	{
		cerr << argv[0] << ": " << myAudio.GetErrorString() << endl;
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
		cout << "Frequency    : " << dec << myEmuConfig.frequency << " Hz" 
            << " (" << ((myEmuConfig.bitsPerSample==SIDEMU_8BIT) ? "8" : "16")
            << "-bit " << ((myEmuConfig.channels==SIDEMU_MONO) ? "mono" : "stereo")
            << ")" << endl;
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
		cerr << argv[0] << ": SID Emulator Engine components not ready." << endl;
		exit(EXIT_ERROR_STATUS);
	}
	// Read out the current settings of the sidtune.
	myTune.getInfo( mySidInfo );
	if ( !myTune )
	{
		cerr << argv[0] << ": " << mySidInfo.statusString << endl;
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
		printError(argv[0],ERR_NOT_ENOUGH_MEMORY);
	    exit(EXIT_ERROR_STATUS);
    }

    exitFlag = false;
    if ((signal(SIGHUP,&mysighandler) == SIG_ERR)
        || (signal(SIGINT,&mysighandler) == SIG_ERR)
        || (signal(SIGQUIT,&mysighandler) == SIG_ERR)
        || (signal(SIGTERM,&mysighandler) == SIG_ERR))
    {
		printError(argv[0],ERR_SIGHANDLER);
	    exit(EXIT_ERROR_STATUS);
    }
        
	cout << "Playing, press ^C to stop ..." << endl;
	for (;;)
	{
		sidEmuFillBuffer(myEmuEngine,myTune,buffer,bufSize);
		myAudio.Play(buffer,bufSize);
        if (exitFlag)
            break;
	}

    myAudio.Reset();
    delete[] buffer;
	exit(0);
}

void printError(char* arg0, int num)
{
    switch (num)
    {
     case ERR_SYNTAX:
        {
            cerr << arg0 << ": command line syntax error" << endl
                << "Try `" << arg0 << " --help' for more information." << endl;
            break;
        }
     case ERR_NOT_ENOUGH_MEMORY:
        {
            cerr << arg0 << ": ERROR: Not enough memory." << endl;
            break;
        }
     case ERR_SIGHANDLER:
        {
            cerr << arg0 << ": ERROR: Could not install signal handler." << endl;
            break;
        }
     default:
        break;
    }
}

void printSyntax(char* arg0)
{
    cout 
        << "Syntax: " << arg0 << " [-<option>...] <datafile>|-" << endl
        << "Options:" << endl
        << " --help|-h    display this screen" << endl
        << " -v           verbose output" << endl
        << " -f<num>      set frequency in Hz (default: 22050)" << endl
        << " -o<num>      set song number (default: preset)" << endl
        << " -a           strict PlaySID song compatibility (read the docs!)" << endl
        << " -a2          bank switching mode (overrides -a)" << endl
#if !defined(DISALLOW_16BIT_SOUND)
        << " -16          enable 16-bit sample mixing" << endl
#endif	  
#if !defined(DISALLOW_STEREO_SOUND)
        << " -s           enable stereo playback" << endl
        << " -ss          enable stereo surround" << endl
        << " -pc          enable centered auto-panning (stereo only)" << endl
#endif
        << " -n           set NTSC clock speed (default: PAL)" << endl
        << " -nf          no SID filter emulation" << endl
        << " -ns          MOS 8580 waveforms (default: MOS 6581)" << endl
        << " -c           force song speed = clock speed (PAL/NTSC)" << endl
        << " -bn<num>     set number of audio buffer fragments to use" << endl
        << " -bs<num>     set size 2^<num> of audio buffer fragments" << endl
        << " -b<num>      set sample buffer size" << endl
        << endl
        << "Mail comments, bug reports, or contributions to <sidplay@geocities.com>." << endl;
}
