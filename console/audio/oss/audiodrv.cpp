// --------------------------------------------------------------------------
// ``Open Sound System (OSS)'' specific audio driver interface.
// --------------------------------------------------------------------------

#include "audiodrv.h"

#if defined(HAVE_NETBSD)
const char audioDriver::AUDIODEVICE[] = "/dev/audio";
#else
const char audioDriver::AUDIODEVICE[] = "/dev/dsp";
#endif

audioDriver::audioDriver()
{
	// Reset everything.
	errorString = "None";
	frequency = 0;
	channels = 0;
	encoding = 0;
	precision = 0;
    swapEndian = false;
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
        perror(AUDIODEVICE);
		errorString = "ERROR: Could not open audio device.";
		return false;
    }
	
	// Transfer input parameters to this object.
	// May later be replaced with driver defaults.
	frequency = inFreq;
	precision = inPrecision;
	channels = inChannels;
	fragments = inFragments;
	fragSizeBase = inFragBase;

    int mask;
    // Query supported sample formats.
    if (ioctl(audioHd,SNDCTL_DSP_GETFMTS,&mask) == (-1))
    {
        perror(AUDIODEVICE);
		errorString = "AUDIO: Could not get sample formats.";
		return false;
    }

    // Assume CPU and soundcard have same endianess.
    swapEndian = false;
    
    int wantedFormat;
	// Set sample precision and type of encoding.
	if (precision == SIDEMU_16BIT)
    {
#if defined(WORDS_BIGENDIAN)
        if (mask & AFMT_S16_BE)
        {
            wantedFormat = AFMT_S16_BE;
            encoding = SIDEMU_SIGNED_PCM;
        }
        else if (mask & AFMT_U16_BE)
        {
            wantedFormat = AFMT_U16_BE;
            encoding = SIDEMU_UNSIGNED_PCM;
        }
        else if (mask & AFMT_S16_LE)
        {
            wantedFormat = AFMT_S16_LE;
            encoding = SIDEMU_SIGNED_PCM;
            swapEndian = true;
        }
        else if (mask & AFMT_U16_LE)
        {
            wantedFormat = AFMT_U16_LE;
            encoding = SIDEMU_UNSIGNED_PCM;
            swapEndian = true;
        }
#else
        if (mask & AFMT_S16_LE)
        {
            wantedFormat = AFMT_S16_LE;
            encoding = SIDEMU_SIGNED_PCM;
        }
        else if (mask & AFMT_U16_LE)
        {
            wantedFormat = AFMT_U16_LE;
            encoding = SIDEMU_UNSIGNED_PCM;
        }
        else if (mask & AFMT_S16_BE)
        {
            wantedFormat = AFMT_S16_BE;
            encoding = SIDEMU_SIGNED_PCM;
            swapEndian = true;
        }
        else if (mask & AFMT_U16_BE)
        {
            wantedFormat = AFMT_U16_BE;
            encoding = SIDEMU_UNSIGNED_PCM;
            swapEndian = true;
        }
#endif
        else  // 16-bit not supported
        {
            wantedFormat = AFMT_U8;
            precision = SIDEMU_8BIT;
            encoding = SIDEMU_UNSIGNED_PCM;
        }
    }
	else  // if (precision == SIDEMU_8BIT)
    {
		wantedFormat = AFMT_U8;
		precision = SIDEMU_8BIT;
		encoding = SIDEMU_UNSIGNED_PCM;
    }

    if ( !(mask&wantedFormat) )
    {
        errorString = "AUDIO: Desired sample encoding not supported.";
        return false;
    }
    
    int format = wantedFormat;
    if (ioctl(audioHd,SNDCTL_DSP_SETFMT,&format) == (-1))
    {
        perror(AUDIODEVICE);
		errorString = "AUDIO: Could not set sample format.";
		return false;
    }
    if (format != wantedFormat)  
    {
		errorString = "AUDIO: Audio driver did not accept sample format.";
		return false;
    }
    
	// Set mono/stereo.
	int dsp_stereo;
	if (channels == SIDEMU_STEREO)
		dsp_stereo = 1;
	else  // if (channels == SIDEMU_MONO)
		dsp_stereo = 0;
	if (ioctl(audioHd,SNDCTL_DSP_STEREO,&dsp_stereo) == (-1))
    {
        perror(AUDIODEVICE);
		errorString = "AUDIO: Could not set mono/stereo.";
		return false;
    }
	// Verify and accept the number of channels the driver accepted.
	if (dsp_stereo == 1)
		channels = SIDEMU_STEREO;
	else if (dsp_stereo == 0)
		channels = SIDEMU_MONO;
	else
    {
		errorString = "AUDIO: Could not set mono/stereo.";
		return false;
    }
	
	// Set frequency.
	int dsp_speed = frequency;
	if (ioctl(audioHd,SNDCTL_DSP_SPEED,&dsp_speed) == (-1))
    {
        perror(AUDIODEVICE);
		errorString = "AUDIO: Could not set frequency.";
		return false;
    }
	// Accept the frequency the driver accepted.
	frequency = dsp_speed;
	
	// N fragments of size (2 ^ S) bytes
	//               NNNNSSSS
	// e.g. frag = 0x0004000e;
	// fragments should be out of [2,3,..,255]
	// fragSizeBase should be out of [7,8,...,17]
	// depending on the kernel audio driver buffer size
	int frag = (fragments << 16) | fragSizeBase;
	ioctl(audioHd,SNDCTL_DSP_SETFRAGMENT,&frag);
	ioctl(audioHd,SNDCTL_DSP_GETBLKSIZE,&blockSize);
	
	audio_buf_info myAudInfo;
	if (ioctl(audioHd,SNDCTL_DSP_GETOSPACE,&myAudInfo) == (-1))
	{
        perror(AUDIODEVICE);
		errorString = "AUDIO: Could not get audio_buf_info.";
		return false;
	}
	fragments = myAudInfo.fragstotal;
	//cout << "fragsize = " << (int)myAudInfo.fragsize << endl;
	//cout << "bytes = " << (int)myAudInfo.bytes << endl;
	
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
        if (swapEndian)
        {
            for (int n=0; n<bufferSize; n+=2)
            {
                ubyte tmp = pBuffer[n+0];
                pBuffer[n+0] = pBuffer[n+1];
                pBuffer[n+1] = tmp;
            }
        }
        write(audioHd,pBuffer,bufferSize);
	}
}
