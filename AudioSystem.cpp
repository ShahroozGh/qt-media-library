#include "AudioSystem.h"



AudioSystem::AudioSystem()
{
}


AudioSystem::~AudioSystem()
{
}

void AudioSystem::initFMOD()
{
	/*
	Create a System object and initialize.
	*/
	result = FMOD::System_Create(&system);
	//ERRCHECK(result);

	result = system->getVersion(&version);
	//ERRCHECK(result);

	if (version < FMOD_VERSION)
	{
		std::cout << "FMOD lib version" << version << "x doesn't match header version" << FMOD_VERSION;
	}

	result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
	//ERRCHECK(result);


	result = system->createStream(songs[SONG_NUMBER].c_str(), FMOD_LOOP_NORMAL | FMOD_ACCURATETIME | FMOD_2D, 0, &sound);
	//ERRCHECK(result);

	if (result != FMOD_OK) {
        std::cout << "CREATE STREAM PROBLEM in init()" << std::endl;

	}

	result = sound->getNumSubSounds(&numsubsounds);
	//ERRCHECK(result);

	if (numsubsounds)
	{
		sound->getSubSound(0, &sound_to_play);
		//ERRCHECK(result);
	}
	else
	{
		sound_to_play = sound;
	}

	//Create DSP for fft
	result = system->getMasterChannelGroup(&mastergroup);
	result = system->createDSPByType(FMOD_DSP_TYPE_FFT, &mydsp);

	result = system->getMasterChannelGroup(&mastergroup);
	result = mastergroup->addDSP(0, mydsp);

	result = mydsp->setBypass(false);
	result = mydsp->setActive(true);

	if (result != FMOD_OK) {
        std::cout << "ACTIVE PROBLEM in init()" << std::endl;

	}
}

void AudioSystem::startPlayback()
{
	/*
	Play the sound.
	*/
	result = system->playSound(sound_to_play, 0, false, &channel);
	if (result != FMOD_OK) {
		std::cout << "COULD NOT PLAY" << std::endl;

	}
	//ERRCHECK(result);
}

void AudioSystem::updateTick()
{
	result = system->update();
}

void AudioSystem::updateFFT()
{
	//FFT
	int channelsPlaying = 0;

	system->getChannelsPlaying(&channelsPlaying);

	
	float val;
	char s[256];
	unsigned int len;
	float *data = 0;
	float freq[32];
	int rate, chan, nyquist;
	int windowsize = 1024;

	result = system->getSoftwareFormat(&rate, 0, 0);

	result = mydsp->setParameterInt(FMOD_DSP_FFT_WINDOWTYPE, FMOD_DSP_FFT_WINDOW_TRIANGLE);

	result = mydsp->setParameterInt(FMOD_DSP_FFT_WINDOWSIZE, windowsize);

	result = mydsp->getParameterFloat(FMOD_DSP_FFT_DOMINANT_FREQ, &val, 0, 0);

	result = mydsp->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void **)&fftparameter, &len, s, 256);
}

std::vector<float> AudioSystem::getFormattedSpectrumData()
{
	std::vector<float> spectrum;

	for (int i = 0; i < 1024; i++) {
		float volVal = fftparameter->spectrum[0][i];
		spectrum.push_back(volVal);

		//Padding
		spectrum.push_back(-1.0f);
		spectrum.push_back(-1.0f);
		spectrum.push_back(-1.0f);
		
	}
    return spectrum;
}

std::vector<float> AudioSystem::getRawSpectrumData()
{
    std::vector<float> spectrum;

    if (channel){
        for (int i = 0; i < 1024; i++) {
            float volVal = fftparameter->spectrum[0][i];
            spectrum.push_back(volVal);
        }
    }
    return spectrum;

}

void AudioSystem::togglePause()
{
	if (channel) {
		bool isPaused;
		result = channel->getPaused(&isPaused);
		result = channel->setPaused(!isPaused);
	}
    else{
        std::cout << "togglePause() returned channel not 1" << std::endl;
    }
}

void AudioSystem::seek(int milliseconds)
{
	if (channel) {
		unsigned int prevPos;
		result = channel->getPosition(&prevPos, FMOD_TIMEUNIT_MS);
		result = channel->setPosition(prevPos + milliseconds, FMOD_TIMEUNIT_MS);
    }
    else{
        std::cout << "seek() returned channel not 1" << std::endl;
    }
}

//Change to return bool if failed to play
void AudioSystem::setAudioFile(std::string audioFilePath)
{
    audioPath = audioFilePath;
    sound->release();

    //Make new stream
    result = system->createStream(audioPath.c_str(), FMOD_LOOP_NORMAL | FMOD_ACCURATETIME | FMOD_2D, 0, &sound);
    //ERRCHECK(result);

    if (result != FMOD_OK) {
        std::cout << "CREATE STREAM PROBLEM in setAudioFile(): " << FMOD_Result_toString(result) << std::endl;
    }

    result = sound->getNumSubSounds(&numsubsounds);
    //ERRCHECK(result);

    if (numsubsounds)
    {
        sound->getSubSound(0, &sound_to_play);
        //ERRCHECK(result);
    }
    else
    {
        sound_to_play = sound;
    }

}

void AudioSystem::setVolume(float percent)
{
    if (channel) {
        result = channel->setVolume(percent);
    }
    else{
        std::cout << "setVolume() returned channel not 1" << std::endl;
    }
}

unsigned int AudioSystem::getLength()
{
    unsigned int length = 0;
    if (channel) {

        result = sound_to_play->getLength(&length, FMOD_TIMEUNIT_MS);
        if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE))
        {
            //ERRCHECK(result);
        }
    }
    else{
        std::cout << "getLength() returned channel not 1" << std::endl;
    }

    return length;

}

unsigned int AudioSystem::getPosition()
{
    unsigned int pos = 0;
    if (channel) {

        result = channel->getPosition(&pos, FMOD_TIMEUNIT_MS);
        if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE))
        {
            //ERRCHECK(result);
        }
    }
    else{
        std::cout << "getPosition() returned channel not 1" << std::endl;
    }

    return pos;
}

void AudioSystem::setPosition(unsigned int ms)
{
    if (channel) {
        result = channel->setPosition(ms, FMOD_TIMEUNIT_MS);
    }
    else{
        std::cout << "setPosition() returned channel not 1" << std::endl;
    }
}

bool AudioSystem::isPaused()
{
    bool paused;
    if (channel) {
        result = channel->getPaused(&paused);
        if (result != FMOD_OK){
            std::cout << "getPaused() returned an error: " << FMOD_Result_toString(result) << std::endl;
        }
    }
    else{
        std::cout << "getPaused() returned channel not 1" << std::endl;
        return false;
    }
    return paused;
}

bool AudioSystem::isPlaying()
{
    bool playing;
    if (channel) {
        result = channel->isPlaying(&playing);
        if (result != FMOD_OK){
            std::cout << "isPlaying() returned an error: " << FMOD_Result_toString(result) << std::endl;
        }
    }
    else{
        //std::cout << "isPlaying() returned channel not 1" << std::endl;
        return false;
    }
    return playing;
}

void AudioSystem::setPaused(bool paused)
{
    if (channel) {
        result = channel->setPaused(paused);
        if (result != FMOD_OK){
            std::cout << "setPaused() returned an error: " << FMOD_Result_toString(result) << std::endl;
        }
    }
    else{
        qDebug() << "setPaused() returned channel not 1";

    }

}

std::string AudioSystem::FMOD_Result_toString(FMOD_RESULT result)
{
    switch(result){
        case FMOD_OK: return "FMOD_OK" ;  break;
        case FMOD_ERR_BADCOMMAND: return "FMOD_ERR_BADCOMMAND" ; break;
        case FMOD_ERR_CHANNEL_ALLOC: return "FMOD_ERR_CHANNEL_ALLOC" ; break;
        case FMOD_ERR_CHANNEL_STOLEN: return "FMOD_ERR_CHANNEL_STOLEN" ; break;
          case FMOD_ERR_DMA: return "FMOD_ERR_DMA" ; break;
          case FMOD_ERR_DSP_CONNECTION: return "FMOD_ERR_DSP_CONNECTION" ; break;
          case FMOD_ERR_DSP_DONTPROCESS: return "FMOD_ERR_DSP_DONTPROCESS" ; break;
          case FMOD_ERR_DSP_FORMAT: return "FMOD_ERR_DSP_FORMAT" ; break;
          case FMOD_ERR_DSP_INUSE: return "FMOD_ERR_DSP_INUSE" ; break;
          case FMOD_ERR_DSP_NOTFOUND: return "FMOD_ERR_DSP_NOTFOUND" ; break;
          case FMOD_ERR_DSP_RESERVED: return "FMOD_ERR_DSP_RESERVED" ; break;
          case FMOD_ERR_DSP_SILENCE: return "FMOD_ERR_DSP_SILENCE" ; break;
          case FMOD_ERR_DSP_TYPE: return "FMOD_ERR_DSP_TYPE" ; break;
          case FMOD_ERR_FILE_BAD: return  "FMOD_ERR_FILE_BAD" ; break;
          case FMOD_ERR_FILE_COULDNOTSEEK: return  "FMOD_ERR_FILE_COULDNOTSEEK" ; break;
          case FMOD_ERR_FILE_DISKEJECTED: return "FMOD_ERR_FILE_DISKEJECTED" ; break;
          case FMOD_ERR_FILE_EOF: return "FMOD_ERR_FILE_EOF" ; break;
          case FMOD_ERR_FILE_ENDOFDATA: return "FMOD_ERR_FILE_ENDOFDATA" ; break;
          case FMOD_ERR_FILE_NOTFOUND: return  "FMOD_ERR_FILE_NOTFOUND" ; break;
          case FMOD_ERR_FORMAT: return  "FMOD_ERR_FORMAT" ; break;
          case FMOD_ERR_HEADER_MISMATCH: return "FMOD_ERR_HEADER_MISMATCH" ; break;
          case FMOD_ERR_HTTP: return "FMOD_ERR_HTTP" ; break;
          case FMOD_ERR_HTTP_ACCESS: return "FMOD_ERR_HTTP_ACCESS" ; break;
          case FMOD_ERR_HTTP_PROXY_AUTH: return "FMOD_ERR_HTTP_PROXY_AUTH" ; break;
          case FMOD_ERR_HTTP_SERVER_ERROR: return "FMOD_ERR_HTTP_SERVER_ERROR" ; break;
          case FMOD_ERR_HTTP_TIMEOUT: return "FMOD_ERR_HTTP_TIMEOUT" ; break;
          case FMOD_ERR_INITIALIZATION: return "FMOD_ERR_INITIALIZATION" ; break;
          case FMOD_ERR_INITIALIZED: return "FMOD_ERR_INITIALIZED" ; break;
          case FMOD_ERR_INTERNAL: return "FMOD_ERR_INTERNAL" ; break;
          case FMOD_ERR_INVALID_FLOAT: return "FMOD_ERR_INVALID_FLOAT" ; break;
          case FMOD_ERR_INVALID_HANDLE: return "FMOD_ERR_INVALID_HANDLE" ; break;
          case FMOD_ERR_INVALID_PARAM: return "FMOD_ERR_INVALID_PARAM" ; break;
          case FMOD_ERR_INVALID_POSITION: return "FMOD_ERR_INVALID_POSITION" ; break;
          case FMOD_ERR_INVALID_SPEAKER: return "FMOD_ERR_INVALID_SPEAKER" ; break;
          case FMOD_ERR_INVALID_SYNCPOINT: return "FMOD_ERR_INVALID_SYNCPOINT" ; break;
          case FMOD_ERR_INVALID_THREAD: return "FMOD_ERR_INVALID_THREAD" ; break;
          case FMOD_ERR_INVALID_VECTOR: return "FMOD_ERR_INVALID_VECTOR" ; break;
          case FMOD_ERR_MAXAUDIBLE: return "FMOD_ERR_MAXAUDIBLE" ; break;
          case FMOD_ERR_MEMORY: return "FMOD_ERR_MEMORY" ; break;
          case FMOD_ERR_MEMORY_CANTPOINT: return "FMOD_ERR_MEMORY_CANTPOINT" ; break;
          case FMOD_ERR_NEEDS3D: return "FMOD_ERR_NEEDS3D" ; break;
          case FMOD_ERR_NEEDSHARDWARE: return "FMOD_ERR_NEEDSHARDWARE" ; break;
          case FMOD_ERR_NET_CONNECT: return "FMOD_ERR_NET_CONNECT" ; break;
          case FMOD_ERR_NET_SOCKET_ERROR: return "FMOD_ERR_NET_SOCKET_ERROR" ; break;
          case FMOD_ERR_NET_URL: return "FMOD_ERR_NET_URL" ; break;
          case FMOD_ERR_NET_WOULD_BLOCK: return "FMOD_ERR_NET_WOULD_BLOCK" ; break;
          case FMOD_ERR_NOTREADY: return "FMOD_ERR_NOTREADY" ; break;
          case FMOD_ERR_OUTPUT_ALLOCATED: return "FMOD_ERR_OUTPUT_ALLOCATED" ; break;
          case FMOD_ERR_OUTPUT_CREATEBUFFER: return "FMOD_ERR_OUTPUT_CREATEBUFFER" ; break;
          case FMOD_ERR_OUTPUT_DRIVERCALL: return "FMOD_ERR_OUTPUT_DRIVERCALL" ; break;
          case FMOD_ERR_OUTPUT_FORMAT: return "FMOD_ERR_OUTPUT_FORMAT" ; break;
          case FMOD_ERR_OUTPUT_INIT: return "FMOD_ERR_OUTPUT_INIT" ; break;
          case FMOD_ERR_OUTPUT_NODRIVERS: return "FMOD_ERR_OUTPUT_NODRIVERS" ; break;
          case FMOD_ERR_PLUGIN: return "FMOD_ERR_PLUGIN" ; break;
          case FMOD_ERR_PLUGIN_MISSING: return "FMOD_ERR_PLUGIN_MISSING" ; break;
          case FMOD_ERR_PLUGIN_RESOURCE: return "FMOD_ERR_PLUGIN_RESOURCE" ; break;
          case FMOD_ERR_PLUGIN_VERSION: return "FMOD_ERR_PLUGIN_VERSION" ; break;
          case FMOD_ERR_RECORD: return "FMOD_ERR_RECORD" ; break;
          case FMOD_ERR_REVERB_CHANNELGROUP: return "FMOD_ERR_REVERB_CHANNELGROUP" ; break;
          case FMOD_ERR_REVERB_INSTANCE: return "FMOD_ERR_REVERB_INSTANCE" ; break;
          case FMOD_ERR_SUBSOUNDS: return "FMOD_ERR_SUBSOUNDS" ; break;
          case FMOD_ERR_SUBSOUND_ALLOCATED: return "FMOD_ERR_SUBSOUND_ALLOCATED" ; break;
          case FMOD_ERR_SUBSOUND_CANTMOVE: return "FMOD_ERR_SUBSOUND_CANTMOVE" ; break;
          case FMOD_ERR_TAGNOTFOUND: return "FMOD_ERR_TAGNOTFOUND" ; break;
          case FMOD_ERR_TOOMANYCHANNELS: return "FMOD_ERR_TOOMANYCHANNELS" ; break;
          case FMOD_ERR_TRUNCATED: return "FMOD_ERR_TRUNCATED" ; break;
          case FMOD_ERR_UNIMPLEMENTED: return "FMOD_ERR_UNIMPLEMENTED" ; break;
          case FMOD_ERR_UNINITIALIZED: return "FMOD_ERR_UNINITIALIZED" ; break;
          case FMOD_ERR_UNSUPPORTED: return "FMOD_ERR_UNSUPPORTED" ; break;
          case FMOD_ERR_VERSION: return "FMOD_ERR_VERSION" ; break;
          case FMOD_ERR_EVENT_ALREADY_LOADED: return "FMOD_ERR_EVENT_ALREADY_LOADED" ; break;
          case FMOD_ERR_EVENT_LIVEUPDATE_BUSY: return "FMOD_ERR_EVENT_LIVEUPDATE_BUSY" ; break;
          case FMOD_ERR_EVENT_LIVEUPDATE_MISMATCH: return "FMOD_ERR_EVENT_LIVEUPDATE_MISMATCH"  ; break;
          case FMOD_ERR_EVENT_LIVEUPDATE_TIMEOUT: return "FMOD_ERR_EVENT_LIVEUPDATE_TIMEOUT" ; break;
          case FMOD_ERR_EVENT_NOTFOUND: return "FMOD_ERR_EVENT_NOTFOUND" ; break;
          case FMOD_ERR_STUDIO_UNINITIALIZED: return "FMOD_ERR_STUDIO_UNINITIALIZED" ; break;
          case FMOD_ERR_STUDIO_NOT_LOADED: return "FMOD_ERR_STUDIO_NOT_LOADED" ; break;
          case FMOD_ERR_INVALID_STRING: return "FMOD_ERR_INVALID_STRING" ; break;
          case FMOD_ERR_ALREADY_LOCKED: return "FMOD_ERR_ALREADY_LOCKED" ; break;
          case FMOD_ERR_NOT_LOCKED: return "FMOD_ERR_NOT_LOCKED" ; break;
          case FMOD_ERR_RECORD_DISCONNECTED: return "FMOD_ERR_RECORD_DISCONNECTED" ; break;
          case FMOD_ERR_TOOMANYSAMPLES: return "FMOD_ERR_TOOMANYSAMPLES" ; break;
    default: return "FMOD_UNLISTED_ERROR"; break;



    }

}

std::string AudioSystem::getCurrentSongPath()
{
    return audioPath;
}
