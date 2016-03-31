#pragma once
#include <SDL_mixer.h>

class Music
{
	friend class AudioManager;

public:
	~Music();

	void Play();
	void Stop();
	void Pause();
	void Resume();

	void setLoops(int);

private:
	Music(Mix_Music * _music, int _numLoops = -1);

	Mix_Music * music;
	int isLooped;
};

