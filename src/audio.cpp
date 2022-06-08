#include "audio.h"
#include <iostream>
std::map<std::string, Audio*> Audio::sLoadedAudios;
Audio::Audio()
{
	hSample = 0;
}

Audio::~Audio()
{
	BASS_SampleFree(hSample);
	if (filename.size())
	{
		auto it = sLoadedAudios.find(filename);
		/*if (it != sLoadedAudios.end())
			sLoadedAudios.erase(it);*/
	}
}
Audio* Audio::Find(const char* filename)
{
	assert(filename);
	auto it = sLoadedAudios.find(filename);
	if (it != sLoadedAudios.end())
		return it->second;
	return NULL;
}
HCHANNEL* Audio::Play()
{

	//El handler para un canal
	HCHANNEL hSampleChannel;
	//Creamos un canal para el sample

	hSampleChannel = BASS_SampleGetChannel(hSample, false);


	//Lanzamos un sample
	BASS_ChannelPlay(hSampleChannel, true);
	return &hSampleChannel;

}
HSAMPLE Audio::LoadSample(const char* filename)
{
	std::string str2 = filename;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	str2.erase(0, 5);
#else
	str2.erase(0, 64);
#endif


	//El handler para un canal
	HCHANNEL hSampleChannel;

	//Cargamos un sample del disco duro (memoria, filename, offset, length, max, flags)
	//use BASS_SAMPLE_LOOP in the last param to have a looped sound
	hSample = BASS_SampleLoad(false, filename, 0, 0, 3, 0);
	setName(filename);
	if (hSample == 0)
	{
		std::cout << "ERROR load" << filename << std::endl;
	}

	std::cout << "AUDIO load " << filename << std::endl;
	return hSample;

}

void Audio::setName(const char* name)
{

	filename = name;
	sLoadedAudios[filename] = this;

}

Audio* Audio::Get(const char* filename)
{
	//load it
	Audio* audio = Find(filename);
	if (audio)
		return audio;

	audio = new Audio();
	if (!audio->LoadSample(filename))
	{
		delete audio;
		return NULL;
	}

	return audio;
}
