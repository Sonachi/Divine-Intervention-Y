//Author:	Mathieu Violette
//Date:		1/18/2015

#pragma once

#include <Level.h>
#include <ModelManager.h>
#include <RenderableComponent.h>
#include <Texture.h>
#include <Light.h>
#include <Billboard.h>
#include <ParticleSystem.h>
#include <Label.h>
#include <FontManager.h>
#include <Flipbook.h>
#include <GuiImage.h>

#include "TestGame.h"

/*
A class that inherits from Level so the user can specify the behaviour of
their game and create their own gameObjects
*/

class TestNetLevel : public Level
{
public:
	union SDLEventxfer
	{
		SDL_Event e;
		char c[sizeof(SDL_Event)];
		Uint8 ui8[sizeof(SDL_Event)/8];
	};

	TestNetLevel();
	~TestNetLevel();

	GameObject *skybox, *ground, *Frosty, *frosty_face, *frosty_nose;
	GameObject *light;

	IPaddress serverIP, *clientIP;
	TCPsocket serverSocket, clientSocket;
	UDPsocket serverSocketUDP, clientSocketUDP;
	UDPpacket *packetUDP;

	int quit, len;
	char buffer[512];
	bool connected, isServer, TCPConnection;

	void LoadContent() override;
	void DebugRender() override;
	void LevelUpdate(float _timeStep) override;

	void RotateFrosty(GameObject *frosty_, SDL_Event e_, float timeStep_);
	float timeSinceLastPacket;
	const float PACKET_COOLDOWN = 1 / 18.0f;
	bool leftClick;
};