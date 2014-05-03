//
// /home/ms/sidplay/audio/RCS/sparc_audiodrv.h,v
//

#ifndef AUDIODRV_H
#define AUDIODRV_H


#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

//#if defined(sun) && defined(sparc)
#if defined(solaris2)
  #include <sys/audioio.h>
#else
  #include <sun/audioio.h>
//  #include <sun/dbriio.h>
#endif
//#endif

#include "mytypes.h"
#include "emucfg.h"

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

	const char AUDIODEVICE[] = "/dev/audio";
	int audioHd;
	
	const char* errorString;
	
	int blockSize;
	
	udword frequency;
	
	// These are constants/enums from ``libsidplay/include/emucfg.h''.
	int encoding;
	int precision;
	int channels;
};


#endif  // AUDIODRV_H
