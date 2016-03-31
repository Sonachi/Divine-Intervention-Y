//Author: Nathan Senter
//Date: 01/12/2015

#pragma once

#include "Component.h"
#include <vector>

class Generic_RenderableComponent;
typedef unsigned int UINT32;

class Flipbook : public Component
{
public:
	enum Image_Extension
	{
		PNG, JPG, JPEG, TIF
	};

	Flipbook(GameObject *, UINT32 _numImages, std::string _filePath, float _playTime, bool _isLooped, Image_Extension _ext);
	~Flipbook();

	void Play();
	void Pause();
	void Stop();

	void UpdateFlipbook(float _deltaTime);

	void SetEffecctedByLight(bool _directional = true, bool _point = true, bool _spot = true);

private:
	bool isRunning;
	bool isLooped;

	float elapsedTime;

	float timePerFrame;
	float timeSinceLastFrame;
	float totalPlayTime;

	unsigned int numFrames;
	unsigned int currentFrame;

	Generic_RenderableComponent * ownerRenderer;

	std::vector<UINT32> textureNames;
};