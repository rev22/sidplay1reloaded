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

#if defined(HAVE_LINUX) && defined(HAVE_LINUX_SOUNDCARD_H)
  #include <linux/soundcard.h>
#elif defined(HAVE_FREEBSD) && defined((HAVE_MACHINE_SOUNDCARD_H)
  #include <machine/soundcard.h>
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
		return (ioctl(audioHd,SNDCTL_DSP_RESET,0)!=(-1));
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
		return fragments;
    }
	
	int GetFragSizeBase()
	{
		return fragSizeBase;
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
	int fragments;
	int fragSizeBase;
	
	udword frequency;
	
	// These are constants/enums from ``libsidplay/include/emucfg.h''.
	int encoding;
	int precision;
	int channels;
};


#endif  // AUDIODRV_H
