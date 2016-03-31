#include "Sound.h"
#include <SDL_mixer.h>

Sound::Sound(Mix_Chunk * _sound)
{
	sound = _sound;
}


Sound::~Sound()
{
	Mix_FreeChunk(sound);
}

void Sound::Play()
{
	Mix_PlayChannel(-1, sound, 0);
}
