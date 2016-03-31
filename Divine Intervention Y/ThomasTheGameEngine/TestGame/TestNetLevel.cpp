//Author:	Mathieu Violette
//Date:		01/14/2016

#include "TestNetLevel.h"

#include <SDL.h>
#include <sstream>
#include <Flipbook.h>
#include <Rigidbody.h>
#include <PhysicsWorld.h>

#include "HeadBob.h"
#include <InputHandler.h>

TestNetLevel::TestNetLevel()
{
}

void TestNetLevel::LoadContent()
{
	currentCamera->position = Vec3(0, 3.5f, -8);
	ambientLightColor = Colour(0.1f, 0.1f, 0.1f);

	Frosty = new GameObject(this, Vec3(0, 1, 0));
	frosty_face = new GameObject(this, Frosty->position);
	frosty_nose = new GameObject(this, Frosty->position);

	light = new GameObject(this, Vec3(0, 60, -60));
	skybox = new GameObject(this, currentCamera->position);
	ground = new GameObject(this, Vec3(0, 0, 0));

	ground->LookAt(ground->position + Vec3(0, 1, 0));
	light->LookAt(Vec3(0, 0, 0));

	Frosty->addChild(frosty_face);
	Frosty->addChild(frosty_nose);
	Frosty->Rotate(Quat(180 * 3.14159f / 180.0f, Vec3(0, 1, 0)));

	/* CREATE MODELS */
	ModelManager::getInstance()->loadModel("frostBody", "Models/frosty_body.obj", false, ModelManager::Draw_Mode::CW);
	ModelManager::getInstance()->loadModel("frostFace", "Models/frosty_face_hat.obj", false, ModelManager::Draw_Mode::CW);
	ModelManager::getInstance()->loadModel("frostNose", "Models/frosty_nose.obj", false, ModelManager::Draw_Mode::CW);

	ModelManager::getInstance()->CreateSkybox("skybox", 200);
	ModelManager::getInstance()->CreatePlane("ground", 100, 100, 32, 32);

	/* CREATE TEXTURES */
	const int texSize = 13 * 13 * 4;
	float pixelDataWhite[texSize];
	for (int i = 0; i < texSize; i++)
	{
		pixelDataWhite[i] = 1.0f;
	}
	Models->createTexture("white", pixelDataWhite, 1, 1);

	float pixelDataBlack[]
	{
		0.1f, 0.1f, 0.1f, 1.0f
	};
	ModelManager::getInstance()->createTexture("black", pixelDataBlack, 1, 1);

	float pixelDataOrange[]
	{
		1, 0.5f, 0, 1
	};
	ModelManager::getInstance()->createTexture("orange", pixelDataOrange, 1, 1);

	ModelManager::getInstance()->loadTexture("grass", "Images/grass.png");
	ModelManager::getInstance()->loadTexture("skybox", "Images/Day_Skybox.png");

	/* Add Components */
	new RenderableComponent("frostBody", "white", Frosty, new Material(1, 1, 1));
	new RenderableComponent("frostFace", "black", frosty_face, new Material(1, 1, 1));
	new RenderableComponent("frostNose", "orange", frosty_nose, new Material(1, 1, 1));

	new RenderableComponent("ground", "grass", ground, new Material(1, 1, 1));
	RenderableComponent *skyRC = new RenderableComponent("skybox", "skybox", skybox);
	skyRC->SetEffecctedByLight(1, 0, 0);

	new Light(light, Colour(1.0f, 1.0f, 1.0f), Light::Directional);


	/* NET STUFF */

	char c = ' ';

	std::cout << "\n";

	std::cout << "TCP or UDP? : T / U\n";

	while (!(c == 'T' || c == 't' || c == 'U' || c == 'u'))
	{
		std::cin >> c;
	}

	if (c == 'T' || c == 't')
		TCPConnection = true;
	else
		TCPConnection = false;

	std::cout << "\n";
	std::cout << "Server or Client? : S / C\n";

	while (!(c == 'S' || c == 's' || c == 'C' || c == 'c'))
	{
		std::cin >> c;
	}

	if (TCPConnection) // TCP
	{

		if (c == 'S' || c == 's')
		{
			isServer = true;
			connected = false;

			SDLNet_ResolveHost(&serverIP, NULL, 27888);
			serverSocket = SDLNet_TCP_Open(&serverIP);

			std::cout << "\nServer Started on Port " << _byteswap_ushort(serverIP.port) << ".\n\n";
		}
		else
		{
			isServer = false;
			std::cout << "\nEnter Server IP Address:\n";
			std::string ip, port;
			std::cin >> ip;
			std::cout << "\nEnter Server Port:\n";
			std::cin >> port;

			int iPort = atoi(port.c_str());

			std::cout << "\nLooking for Host...\n";

			bool hostFound = false;
			while (!hostFound)
			{
				if (SDLNet_ResolveHost(&serverIP, ip.c_str(), iPort) == 0)
				{
					do
					{
						serverSocket = SDLNet_TCP_Open(&serverIP);
					} while (serverSocket == NULL);

					hostFound = true;
				}
			}

			std::cout << "Host Found.\n";
		}
	}
	else // UDP
	{
		if (c == 'S' || c == 's')
		{
			isServer = true;

			SDLNet_ResolveHost(&serverIP, NULL, 27888);
			serverSocketUDP = SDLNet_UDP_Open(27888);
			packetUDP = SDLNet_AllocPacket(56);

			std::cout << "\nServer Started on Port " << _byteswap_ushort(serverIP.port) << ".\n\n";
		}
		else
		{
			isServer = false;
			std::cout << "\nEnter Server IP Address:\n";
			std::string ip, port;
			std::cin >> ip;
			std::cout << "\nEnter Server Port:\n";
			std::cin >> port;

			int iPort = atoi(port.c_str());

			serverSocketUDP = SDLNet_UDP_Open(0);
			SDLNet_ResolveHost(&serverIP, ip.c_str(), iPort);
			packetUDP = SDLNet_AllocPacket(56);

			SDL_SetRelativeMouseMode(SDL_TRUE); //Traps Mouse in Window (centre)
		}
	}

	timeSinceLastPacket = 0;
}

TestNetLevel::~TestNetLevel()
{
	if (TCPConnection)
	{
		if (isServer)
			SDLNet_TCP_Close(clientSocket);
	}
	else
	{
		SDLNet_FreePacket(packetUDP);
	}

	SDLNet_TCP_Close(serverSocket);
	SDLNet_Quit();
}

void TestNetLevel::LevelUpdate(float _timeStep)
{
	Level::LevelUpdate(_timeStep);

	//Frosty->Rotate(Quat(90 * _timeStep * 3.14159f / 180.0f, Vec3(0, 1, 0)));

	//std::stringstream ss;
	//ss << "FPS: " << Game::GetInstance()->GetFPS();
	//fpsLabel->SetText(ss.str());

	skybox->position = currentCamera->position;

	if (TCPConnection) // TCP
	{
		if (isServer)
		{
			{
				/* Now we can communicate with the client using csd socket
				* sd will remain opened waiting other connections */

				/* Get the remote address */
				if (!connected &&
				(clientSocket = SDLNet_TCP_Accept(serverSocket)) &&
				(clientIP = SDLNet_TCP_GetPeerAddress(clientSocket)))
				{
					/* Print the address, converting in the host format */
					printf("Client connected: %x %d\n", SDLNet_Read32(&clientIP->host), SDLNet_Read16(&clientIP->port));

					connected = true;
				}

				if (connected && SDLNet_TCP_Recv(clientSocket, buffer, 512) > 0)
				{
					SDLEventxfer x;

					for (int i = 0; i < sizeof(SDL_Event); i++)
						x.c[i] = buffer[i];

					printf("Client says: X:%d, Y:%d\n", x.e.motion.x, x.e.motion.y);
					//printf("Client says: %s\n", buffer);

					//if (strcmp(buffer, "exit") == 0)	/* Terminate this connection */
					//{
					//	printf("Terminate connection\n");
					//}
					//if (strcmp(buffer, "quit") == 0)	/* Quit the program */
					//{
					//	printf("Quit program\n");
					//}				
				}
			}
		}
		else //isClient
		{
			printf("Write something:\n>");
			std::cin.getline(buffer, 512);

			/*SDLEventxfer x;
			x.e = SDL_Event();
			x.e = InputController::getInstance()->previousEvent;*/

			{

				/*for (int i = 0; i < sizeof(SDL_Event); i++)
					buffer[i] = x.c[i];*/

				len = strlen(buffer) + 1;
				//len = 56;

				if (SDLNet_TCP_Send(serverSocket, (void *)buffer, len) < len)
				{
					std::cout << "SDLNet_TCP_Send: " << SDLNet_GetError() << "\n";
				}
			}
		}
	}
	else //UDP
	{
		if (isServer)
		{
			{
				if (SDLNet_UDP_Recv(serverSocketUDP, packetUDP))
				{
					/*printf("UDP Packet incoming\n");
					printf("\tChan:    %d\n", packetUDP->channel);
					printf("\tData:    %s\n", (char *)packetUDP->data);
					printf("\tLen:     %d\n", packetUDP->len);
					printf("\tMaxlen:  %d\n", packetUDP->maxlen);
					printf("\tStatus:  %d\n", packetUDP->status);
					printf("\tAddress: %x %x\n", packetUDP->address.host, packetUDP->address.port);*/

					//std::cout << "> " << (char *)packetUDP->data << std::endl;


					SDLEventxfer x;

					for (int i = 0; i < sizeof(SDL_Event); i++)
						x.c[i] = ((char *)packetUDP->data)[i];

					RotateFrosty(Frosty, x.e, _timeStep);
					printf("Client: X:%d, Y:%d\n", x.e.motion.x, x.e.motion.y);
				}
			}
		}
		else //isClient
		{
			/*std::cout  << "Write something:\n>";
			std::cin.getline(buffer, 56);*/

			timeSinceLastPacket += _timeStep;

			if (timeSinceLastPacket >= PACKET_COOLDOWN)
			{
				SDLEventxfer x;
				x.e.motion = InputController::getInstance()->motionEvent;

				if (!(x.e.motion.x < 0 || x.e.motion.y < 0))
				{

					timeSinceLastPacket = 0;

					//packetUDP->data = (Uint8 *)buffer;
					packetUDP->data = (Uint8 *)x.c;


					packetUDP->address.host = serverIP.host;	/* Set the destination host */
					packetUDP->address.port = serverIP.port;	/* And destination port */

					packetUDP->len = sizeof(SDL_Event);
					//packetUDP->len = strlen((char *)packetUDP->data) + 1;
					SDLNet_UDP_Send(serverSocketUDP, -1, packetUDP); /* This sets the p->channel */

					RotateFrosty(Frosty, x.e, _timeStep);
				}
			}
		}
	}

}

void TestNetLevel::DebugRender()
{

}

void TestNetLevel::RotateFrosty(GameObject *frosty_, SDL_Event e_, float timeStep_)
{
	//if (e_.motion.xrel != 0)
	{
		frosty_->Rotate(Quat(e_.motion.xrel * timeStep_, Vec3::BasisY()));
	}

	//if (e_.motion.yrel != 0)
	{
		frosty_->Rotate(Quat(e_.motion.yrel * timeStep_ * Vec3::cross(frosty_->forward(), frosty_->up()).length(), Vec3::cross(frosty_->forward(), frosty_->up())));
	}
}
