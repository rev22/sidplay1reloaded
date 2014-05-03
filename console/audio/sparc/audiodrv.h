//
// /home/ms/sidplay/audio/RCS/sparc_audiodrv.h,v
//

#ifndef AUDIODRV_H
#define AUDIODRV_H


#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "compconf.h"
#include "mytypes.h"
#include "emucfg.h"

#if defined(HAVE_SOLARIS) && defined(HAVE_SUN_AUDIOIO_H)
  #include <sun/audioio.h>
//  #include <sun/dbriio.h>
#elif defined(HAVE_SOLARIS) && defined(HAVE_SYS_AUDIOIO_H)
  #include <sys/audioio.h>
#elif defined(HAVE_SUNOS) && defined(HAVE_SUN_AUDIOIO_H)
  #include <sun/audioio.h>
//  #include <sun/dbriio.h>
#elif defined(HAVE_SUNOS) && defined(HAVE_SYS_AUDIOIO_H)
  #include <sys/audioio.h>
#else
  #error Audio driver not supported.
#endif

class audioDriver
{
	
 public:  // --------------------------------------------------------- public

    audioDriver();
  
	bool IsThere();
	
	bool Open(udword freq, int precision, int channels,
			  int fragments, int fragBase);
	
	void Close();
	
	void Play(ubyte* buffer, int bufferSize);

	bool Reset()
	{
		return false;  // not available
	}
	
	int GetAudioHandle()
	{
		return audioHd;
	}
	
	udword GetFrequency()
    { 
		return frequency;
    }
	
	int GetChannels()
    {
		return channels;
    }
	
	int GetSamplePrecision()
    {
		return precision;
    }
	
	int GetSampleEncoding()
    {
		return encoding;
    }
	
	int GetBlockSize()
	{
		return frequency;  //blockSize;
	}
	
	int GetFragments()
    {
		return 1;  //fragments;
    }
	
	int GetFragSizeBase()
    {
		return 0;  //fragSizeBase;
    }
	
	const char* GetErrorString()
	{
		return errorString;
	}
			
 private:  // ------------------------------------------------------- private

	static const char AUDIODEVICE[];
	int audioHd;
	
	char* errorString;
	
	int blockSize;
	
	udword frequency;
	
	// These are constants/enums from ``libsidplay/include/emucfg.h''.
	int encoding;
	int precision;
	int channels;
};


#endif  // AUDIODRV_H
