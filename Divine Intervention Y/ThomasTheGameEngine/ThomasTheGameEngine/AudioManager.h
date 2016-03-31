#pragma once
#include <map>
#include <string>
#include<SDL_mixer.h>

#include "Sound.h"
#include "Music.h"

#define Audio AudioManager::getInstance()

class AudioManager
{
	//Friended with gameproperties so that when changes are applied to the properties
	//the private setvolume methods are called.
	friend class GameProperties;

public:
	~AudioManager();

	static AudioManager * instance;

	//Class is a singleton. This method makes it so!
	static AudioManager * getInstance()
	{
		if (!instance) instance = new AudioManager();
		return instance;
	}

	//Load a sound into the map with the given identifier
	void loadSound(std::string _id, std::string _fileName);

	//Returns a sound with the given identifier
	Sound * getSound(std::string _id);

	//Loads a music into the map with the given identifier
	void loadMusic(std::string _id, std::string _fileName);

	//Returns a music from the map with the given identifier
	Music * getMusic(std::string _id);

	static bool isMusicPlaying();

	//Private methods to change volume levels.
	//Should only be accessed by GameProperties
	void setSoundVolume(float _volume);
	void setMusicVolume(float _volume);
private:
	AudioManager();

	//Maps to hold both Sounds and Musics
	//Mapped with a string that acts as an identifier
	std::map<std::string, Sound *> Sounds;
	std::map<std::string, Music *> Musics;



};

