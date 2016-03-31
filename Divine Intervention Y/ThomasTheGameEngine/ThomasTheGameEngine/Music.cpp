#include "Music.h"

Music::Music(Mix_Music * _music, int _numLoops)
{
	music = _music;
	isLooped = _numLoops;
}

Music::~Music()
{
	Mix_FreeMusic(music);
}

void Music::Play()
{
	if (Mix_PlayingMusic() == 0)
	{
		Mix_PlayMusic(music, isLooped);
	}
	else
	{
		Mix_HaltMusic();
		Mix_PlayMusic(music, isLooped);
	}
}

void Music::Stop()
{
	if (Mix_PlayingMusic() != 0) Mix_HaltMusic();
}

void Music::Pause()
{
	Mix_PauseMusic();
}

void Music::Resume()
{
	if (Mix_PausedMusic() == 1) Mix_ResumeMusic();
}

void Music::setLoops(int _loops)
{
	isLooped = _loops;
}
