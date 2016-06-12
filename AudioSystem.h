#pragma once
#include <vector>
#include <string>
#include <iostream>

#include <fmod.hpp>
#include <QDebug>

#define SONG_NUMBER 3

class AudioSystem
{
public:
	AudioSystem();
	~AudioSystem();

	std::string songs[4] = { "C:/Users/Shahrooz/Documents/Visual Studio 2015/Projects/FreqSpectrumVisualizer/FreqSpectrumVisualizer/Knife Party - CentipedeNOINTRO.mp3" ,
		"C:/Users/Shahrooz/Music/RHCP/Red_Hot_Chili_Peppers_-_Dani_California_[Official_Music_Video].mp3",
		"C:/Users/Shahrooz/Music/City and Colour/City And Colour - The Hurry And The Harm 2013 Funk 320kbps CBR MP3 [VX]/04 City and Colour - The Lonely Life.mp3",
		"C:/Users/Shahrooz/Music/Muse - Drones (2015) 320 Kbps + Digital Booklet -AryaN_L33T-[GloDLS]/05 Reapers.mp3" };

    std::string audioPath;

	//FMOD stuff
	FMOD::System     *system;
	FMOD::Sound      *sound, *sound_to_play;
	FMOD::Channel    *channel = 0;
	FMOD_RESULT       result;
	unsigned int      version;
	void             *extradriverdata = 0;
	int               numsubsounds;

	//For DSP FFt
	FMOD::ChannelGroup *mastergroup;
	FMOD::DSP *mydsp;

	FMOD_DSP_PARAMETER_FFT *fftparameter;
	
	void initFMOD();
	void startPlayback();

	//Seek + pause accessors

	//Fmod Audio tick
	void updateTick();

	void updateFFT();

	std::vector<float> getFormattedSpectrumData();

	//Control Functions
	void togglePause();

	void seek(int milliseconds);

    void setAudioFile(std::string audioFilePath);

    void setVolume(float percent);

    //Get length of track
    unsigned int getLength();

    //Get position of track
    unsigned int getPosition();

    void setPosition(unsigned int ms);

    bool isPaused();
    bool isPlaying();

    void setPaused(bool paused);

    std::string getCurrentSongPath();




};

