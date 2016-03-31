#pragma once

struct Mix_Chunk;

class Sound
{
	friend class AudioManager;

public:
	~Sound();

	void Play();

private:
	//Sounds must be created via the AudioManager
	Sound(Mix_Chunk * _sound);

	Mix_Chunk * sound;
};

