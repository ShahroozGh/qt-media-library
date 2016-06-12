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
		std::cout << "CREATE STREAM PROBLEM" << std::endl;
		while (true)
		{

		}
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
		std::cout << "ACTIVE PROBLEM" << std::endl;
		while (true)
		{

		}
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
		while (true)
		{

		}
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

void AudioSystem::setAudioFile(std::string audioFilePath)
{
    audioPath = audioFilePath;
    sound->release();

    //Make new stream
    result = system->createStream(audioPath.c_str(), FMOD_LOOP_NORMAL | FMOD_ACCURATETIME | FMOD_2D, 0, &sound);
    //ERRCHECK(result);

    if (result != FMOD_OK) {
        std::cout << "CREATE STREAM PROBLEM" << std::endl;
        while (true)
        {

        }
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
            std::cout << "getPaused() returned an error" << std::endl;
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
            std::cout << "isPlaying() returned an error" << std::endl;
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
            qDebug() << "setPaused() returned an error";
        }
    }
    else{
        qDebug() << "setPaused() returned channel not 1";

    }

}

std::string AudioSystem::getCurrentSongPath()
{
    return audioPath;
}
