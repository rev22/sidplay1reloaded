//
// 1997/09/27 21:33:14
//

#ifndef __SIDTUNE_H
 #define __SIDTUNE_H


#include <fstream.h>
#include "mytypes.h"

static const int classMaxSongs = 256;
static const int infoStringNum = 5;     // maximum
static const int infoStringLen = 80+1;  // 80 characters plus terminating zero

static const int SIDTUNE_SPEED_CIA_1A = 0;     // CIA 1 Timer A
static const int SIDTUNE_SPEED_VBI_PAL = 50;   // Hz
static const int SIDTUNE_SPEED_VBI_NTSC = 60;  // Hz

static const int SIDTUNE_CLOCK_PAL = 0;   // These are also used in the
static const int SIDTUNE_CLOCK_NTSC = 1;  // emulator engine !

// An instance of this structure is used to transport values to
// and from the ``sidTune-class'';
struct sidTuneInfo
{
	// Consider the following fields as read-only !
	//
	// Currently, the only way to get the class to accept values which
	// were written to these fields is creating a derived class.
	//
	const char* formatString;   // the name of the identified file format
	const char* speedString;    // describing the speed a song is running at
	uword loadAddr;
	uword initAddr;
	uword playAddr;
	uword startSong;
	uword songs;
	//
	// Available after song initialization.
	//
	uword irqAddr;              // if (playAddr == 0), interrupt handler was
	                            // installed and starts calling the C64 player
	                            // at this address
	uword currentSong;          // the one that has been initialized
	ubyte songSpeed;            // intended speed, see top
	ubyte clockSpeed;           // -"-
	bool musPlayer;             // true = install sidplayer routine
	uword lengthInSeconds;      // --- not yet supported ---
	//
	// Song title, credits, ...
	//
	ubyte numberOfInfoStrings;  // the number of available text info lines
	char* infoString[infoStringNum];
	char* nameString;           // name, author and copyright strings
	char* authorString;         // are duplicates of infoString[?]
	char* copyrightString;
	//
	uword numberOfCommentStrings;  // --- not yet supported ---
	char ** commentString;         // -"-
	//
	udword dataFileLen;         // length of single-file sidtune or raw data
	udword c64dataLen;          // length of raw C64 data
	char* dataFileName;         // a first file: e.g. ``*.DAT''
	char* infoFileName;         // a second file: e.g. ``*.SID''
	//
	const char* statusString;   // error/status message of last operation
};


class emuEngine;


class sidTune
{

 public:  // ----------------------------------------------------------------

	// --- constructors ---

	// Load a sidtune from a file.
	// To retrieve data from standard input pass in filename "-".
	sidTune(const char* sidTuneFileName);

	// If you want to override the default filename extensions use this
	// contructor. Please note, that if the specified ``sidTuneFileName''
	// does exist and the loader is able to determine its file format,
	// this function does not try to append any file name extension.
	// See ``sidtune.cpp'' for the default list of file name extensions.
	// You can specific ``sidTuneFileName = 0'', if you do not want to
	// load a sidtune. You can later load one with open().
	sidTune(const char* sidTuneFileName, const char **fileNameExt);

	// Load a single-file sidtune from a memory buffer.
	// Currently supported: PSID format
	sidTune(ubyte* oneFileFormatSidtune, udword sidtuneLength);

    virtual ~sidTune();  // destructor

	// --- member functions ---

	// Load a sidtune from a file into an existing object.
	bool open(const char* sidTuneFileName);
    bool load(ubyte* oneFileFormatSidtune, udword sidtuneLength);

	bool getInfo( struct sidTuneInfo& );
	bool returnInfo( struct sidTuneInfo& outSidTuneInfo )  { return getInfo(outSidTuneInfo); }
    virtual bool setInfo( struct sidTuneInfo& );  // dummy

    ubyte getSongSpeed()  { return info.songSpeed; }
    ubyte returnSongSpeed()  { return getSongSpeed(); }

	uword getPlayAddr()	{ return info.playAddr;	}
	uword returnPlayAddr()	{ return getPlayAddr(); }

    // This function initializes the SID emulator engine to play the given
    // sidtune song.
	friend bool sidEmuInitializeSong(emuEngine &, sidTune &, uword songNum);

    // This is an old non-obsolete sub-function, that does not scan the sidtune
    // for digis. If (emuConfig.digiPlayerScan == 0), this functions does the
	// same as the one above.
	friend bool sidEmuInitializeSongOld(emuEngine &, sidTune &,	uword songNum);

	// Determine current state of object (true = okay, false = error).
	// Upon error condition use ``getInfo'' to get a descriptive
	// text string in ``sidTuneInfo.statusString''.
    operator bool()  { return status; }
	bool getStatus()  { return status; }
	bool returnStatus()  { return getStatus(); }

	// --- format conversion ---

	// These functions work for any successfully created object.
	// overWriteFlag: true  = Overwrite existing file.
	//                false = Default, return error when file already
	//                        exists.
	// One could imagine an "Are you sure ?"-checkbox before overwriting
	// any file.
	// returns: true = Successful, false = Error condition.
    bool saveC64dataFile( const char* destFileName, bool overWriteFlag = false );
	bool saveSIDfile( const char* destFileName, bool overWriteFlag = false );
	bool savePSIDfile( const char* destFileName, bool overWriteFlag = false );
	bool saveSID2file( const char* destFileName, bool overWriteFlag = false );


 protected:  // -------------------------------------------------------------

	bool status;
	sidTuneInfo info;

	ubyte songSpeed[classMaxSongs];
	uword songLength[classMaxSongs];   // song lengths in seconds

	// holds text info from the format headers etc.
	char infoString[infoStringNum][infoStringLen];

	ubyte fillUpWidth;  // fill up saved text strings up to this width

	bool isCached;
	ubyte* cachePtr;
	udword cacheLen;

	// Using external buffers for loading files instead of the interpreter
	// memory. This separates the sidtune objects from the interpreter.
	ubyte* fileBuf;
	ubyte* fileBuf2;

	udword fileOffset;  // for files with header: offset to real data

	// Filename extensions to append for various file types.
	const char **fileNameExtensions;

	// --- protected member functions ---

	// Convert 32-bit PSID-style speed word to internal variables.
	void convertOldStyleSpeedToTables(udword oldStyleSpeed);

	// Copy C64 data from internal cache to C64 memory.
	bool placeSidTuneInC64mem( ubyte* c64buf );

	udword loadFile(const char* fileName, ubyte** bufferRef);
	bool saveToOpenFile( ofstream& toFile, ubyte* buffer, udword bufLen );

	// Data caching.
	bool cacheRawData( void* sourceBuffer, udword sourceBufLen );
	bool getCachedRawData( void* destBuffer, udword destBufLen );

	// Support for various file formats.

	virtual bool PSID_fileSupport(void* buffer, udword bufLen);
	virtual bool PSID_fileSupportSave(ofstream& toFile, ubyte* dataBuffer);

	virtual bool MUS_fileSupport(void* buffer, udword bufLen);
	virtual void MUS_installPlayer(ubyte *c64buf);

	virtual bool INFO_fileSupport(void* dataBuffer, udword dataBufLen,
								  void* infoBuffer, udword infoBufLen);

	virtual bool SID_fileSupport(void* dataBuffer, udword dataBufLen,
								 void* sidBuffer, udword sidBufLen);
	virtual bool SID_fileSupportSave(ofstream& toFile);
	
//	virtual bool SID2_fileSupport(void* dataBuffer, udword dataBufLen);
//	virtual bool SID2_fileSupportSave(ofstream& toFile, ubyte* dataBuffer);

	
 private:  // ---------------------------------------------------------------
	
	// --- private member functions ---

	void safeConstructor();
	void safeDestructor();
#if !defined(__POWERPC__)
	void stdinConstructor();
#endif
	void filesConstructor( const char* );   
	
	uword selectSong(uword selectedSong);
	void setIRQaddress(uword address);
	
	void deleteFileBuffers();
	// Try to retrieve single-file sidtune from specified buffer.
	bool getSidtuneFromFileBuffer(ubyte* buffer, udword bufferLen);
	// Cache the data of a single-file or two-file sidtune and its
	// corresponding file names.
	void acceptSidTune(const char* dataFileName, const char* infoFileName,
					   ubyte* dataFileBuf, udword dataLen );
	bool createNewFileName( char** destStringPtr,
						   const char* sourceName, const char* sourceExt);
};
	

#endif
