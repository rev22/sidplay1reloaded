//
// 1997/09/27 21:38:01
//

#include "audiodrv.h"

audioDriver::audioDriver()
{
	// Reset everything.
	errorString = "None";
	frequency = 0;
	channels = 0;
	encoding = 0;
	precision = 0;
	audio = (config = NULL);
}

bool audioDriver::IsThere()
{
	// Check device availability and write permissions.
	return (access(AUDIODEVICE,W_OK)==0);
}

bool audioDriver::Open(udword inFreq, int inPrecision, int inChannels,
					   int inFragments, int inFragBase)
{
	// Transfer input parameters to this object.
	// May later be replaced with driver defaults.
	frequency = inFreq;

	long chpars[] = {AL_OUTPUT_RATE, 0};
 
	// Frequency
 
	chpars[1] = frequency;
	ALsetparams(AL_DEFAULT_DEVICE, chpars, 2);
	ALgetparams(AL_DEFAULT_DEVICE, chpars, 2);
	config = ALnewconfig();
 
	// Set sample format
 
	ALsetsampfmt(config, AL_SAMPFMT_TWOSCOMP);
	encoding = SIDEMU_SIGNED_PCM;  // unlike other systems
	
	// Mono output
 
	ALsetchannels(config, AL_MONO);
	channels = SIDEMU_MONO;
 
	// 8-bit sampleSize
 
	ALsetwidth(config, AL_SAMPLE_8);
	precision = SIDEMU_8BIT;
 
	// Allocate an audio port
 
	if((audio = ALopenport("SIDPLAY sound", "w", config)) == NULL)
    {
		errorString = "AUDIO: Could not open audio port.";
		oserror();
		ALfreeconfig(config);
		return false;  //(1);
    }
 
	// Allocate sound buffers
	blockSize = frequency;
	ALsetqueuesize(config,blockSize);
	
	return true;
}

// Close an opened audio device, free any allocated buffers and
// reset any variables that reflect the current state.
void audioDriver::Close()
{
	if (audio != NULL)
    {
		ALcloseport(audio);
		ALfreeconfig(config);
		audio = (config = NULL);
    }
}

void audioDriver::Play(ubyte* pBuffer, int bufferSize)
{
	if (audio != NULL)
	{
		ALwritesamps(audio,pBuffer,bufferSize);
	}
}
