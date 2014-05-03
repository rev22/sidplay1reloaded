//
// 1997/09/27 21:38:01
//

#include "audiodrv.h"

const char audioDriver::AUDIODEVICE[] = "/dev/audio";

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
					   int inFragments, int inFragSizeBase)
{
	if ((audioHd=open(AUDIODEVICE,O_WRONLY,0)) == (-1))
    {
		errorString = "AUDIO: Could not open audio device.";
		return false;
    }

	// Transfer input parameters to this object.
	// May later be replaced with driver defaults.
	frequency = inFreq;
	channels = inChannels;
	precision = inPrecision;

#ifdef CHECKFORSPEAKERBOX  
	int hwdevice;
	if ( ioctl(audioHd,AUDIO_GETDEV,&hwdevice) == -1)
    {
		errorString = "AUDIO: No audio hardware device installed.";
		return false;
    }
	if ( hwdevice != AUDIO_DEV_SPEAKERBOX )
    {
		audioHd = -1;
		errorString = "AUDIO: Speakerbox not installed/enabled.";
		return false;
    }
#endif
	
	// Choose the nearest possible frequency.
	int dbrifreqs[] = 
	{
		8000, 9600, 11025, 16000, 18900, 22050, 32000, 37800, 44100, 48000, 0
	};
	int dbrifsel = 0;
	int dbrifreqdiff = 100000;
	int dbrifrequency = frequency;
	do
    {  
		int dbrifreqdiff2 = frequency - dbrifreqs[dbrifsel];
		dbrifreqdiff2 < 0 ? dbrifreqdiff2 = 0 - dbrifreqdiff2 : dbrifreqdiff2 += 0;
		if ( dbrifreqdiff2 < dbrifreqdiff )  
		{
			dbrifreqdiff = dbrifreqdiff2;
			dbrifrequency = dbrifreqs[dbrifsel];
		}
		dbrifsel++;
    }  while ( dbrifreqs[dbrifsel] != 0 );
	frequency = dbrifrequency;
	
	precision = SIDEMU_16BIT;  // Only poor audio capabilities for 8-bit.
	// Sparcstations 5 and 10 tend to be 16-bit only at rates above 8000 Hz.
	encoding = SIDEMU_SIGNED_PCM;
	blockSize = frequency;  // dumb mode
	audio_info myaudio_info;
	if ( ioctl(audioHd,AUDIO_GETINFO,&myaudio_info) == (-1))
    {
		errorString = "AUDIO: Could not get audio info.";
		return false;
    }
	AUDIO_INITINFO( &myaudio_info );
	myaudio_info.play.sample_rate = frequency;
	//myaudio_info.play.buffer_size = blockSize;  // does not work ?
	if (channels == SIDEMU_STEREO)
		myaudio_info.play.channels = 2;
	else  // if (channels == SIDEMU_MONO)
		myaudio_info.play.channels = 1;
	myaudio_info.play.precision = precision;
	myaudio_info.play.encoding = AUDIO_ENCODING_LINEAR;
	myaudio_info.output_muted = 0;
	if ( ioctl(audioHd,AUDIO_SETINFO,&myaudio_info) == (-1))
    {
		errorString = "AUDIO: Could not set audio info.";
		return false;
    }

	return true;
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
