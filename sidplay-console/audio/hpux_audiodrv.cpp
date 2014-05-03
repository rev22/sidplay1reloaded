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
	audioHd = (-1);
}

bool audioDriver::IsThere()
{
	// Check device availability and write permissions.
	return (access(AUDIODEVICE,W_OK)==0);
}

bool audioDriver::Open(udword inFreq, int inPrecision, int inChannels,
					   int inFragments, int inFragBase)
{
	if ((audioHd=open(AUDIODEVICE,O_WRONLY,0)) == (-1))
    { 
		errorString = "AUDIO: Could not open audio device.";
		return false;
    }
	
	// Transfer input parameters to this object.
	// May later be replaced with driver defaults.
	frequency = inFreq;
	precision = inPrecision;
	channels = inChannels;
	
	int tmp_sampleSize;
	if (precision == SIDEMU_16BIT)
    {
		tmp_sampleSize = AUDIO_FORMAT_LINEAR16BIT;
		encoding = SIDEMU_SIGNED_PCM;
    }
	else  // if (precision == SIDEMU_8BIT)
    {
		//errorString = "AUDIO: 8-bit samples not supported.";
		//return false;
		precision = SIDEMU_16BIT;
		encoding = SIDEMU_SIGNED_PCM;
		tmp_sampleSize = AUDIO_FORMAT_LINEAR16BIT;
    }
	if (ioctl(audioHd,AUDIO_SET_DATA_FORMAT,tmp_sampleSize) == (-1))
    {
		errorString = "AUDIO: Could not set data format.";
		return false;
    }

	int tmp_stereo;
	if (channels == SIDEMU_STEREO)
		tmp_stereo = 2;
	else  // if (channels == SIDEMU_MONO)
		tmp_stereo = 1;
	if (ioctl(audioHd,AUDIO_SET_CHANNELS,tmp_stereo) == (-1))
    {
		errorString = "AUDIO: Could not set mono/stereo.";
		return false;
    }

	int tmp_speed = frequency;
	if (ioctl(audioHd,AUDIO_SET_SAMPLE_RATE,tmp_speed) == (-1))
    {
		errorString = "AUDIO: Could not set sample rate.";
		return false;
    }
	blockSize = 2*tmp_stereo*frequency;  // dump mode
	
    return true
}

// Close an opened audio device, free any allocated buffers and
// reset any variables that reflect the current state.
void audioDriver::Close()
{
	if (audioHd != (-1))
    {
		close(audioHd);
		audioHd = (-1);
    }
}

void audioDriver::Play(ubyte* pBuffer, int bufferSize)
{
	if (audioHd != (-1))
	{
		write(audioHd,pBuffer,bufferSize);
	}
}
