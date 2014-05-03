//
// 1997/09/27 21:38:01
//

#ifndef AUDIODRV_H
#define AUDIODRV_H


#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "compconf.h"
#include "mytypes.h"
#include "emucfg.h"

#if defined(HAVE_HPUX) && defined(HAVE_SYS_AUDIO_H)
  #include <sys/audio.h>
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
		return blockSize;
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
