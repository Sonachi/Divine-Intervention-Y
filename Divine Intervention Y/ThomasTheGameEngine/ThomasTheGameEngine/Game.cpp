//Author:	Mathieu Violette
//Date:		10/22/2015

#include "Game.h"
#include "Level.h"
#include "GameObject.h"
#include "GameProperties.h"
#include "AudioManager.h"
#include "ModelManager.h"
#include "Camera.h"
#include "OpenGLUtilities.h"
#include "FontManager.h"
#include "PhysicsWorld.h"
#include "InputHandler.h"
#include "RenderableComponent.h"

Game* Game::instance = nullptr;

int timeStart, timeSinceLastFPSUpdate, timeSinceLastUpdate, updatesCount;
float deltaTime;
float FPS_CAP;

Game::Game()
{
	instance = this;

	/* Game Loop Flag */
	isRunning = true;
	callInitOnNextUpdate = false;
	levelToLoad = nullptr;
	lastUpdateTime = 0; timeSincelastUpdate = 0; totalTime = 0;

	///SDL stuff
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
	SDLNet_Init();
	TTF_Init();
	Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);

	//Creates the audio manager which additionally initializes SDL audio
	audioManager = AudioManager::getInstance();
	properties = GameProperties::getInstance();

	SDL_RendererInfo displayRendererInfo;

	SDL_CreateWindowAndRenderer(properties->getVideoProperties()->screenWidth, properties->getVideoProperties()->screenHeight,
		SDL_WINDOW_OPENGL, &gameWindow, &gameRenderer);

	if (properties->getVideoProperties()->isFullscreen) SDL_SetWindowFullscreen(gameWindow, SDL_WINDOW_FULLSCREEN);

	SDL_GetRendererInfo(gameRenderer, &displayRendererInfo);

	if ((displayRendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 ||
		(displayRendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
		/* We have no render surface and not accelerated. */
		printf("\n NO RENDER SURFACE \n");
	}

	/* creates a context for OpenGL draw calls */
	glcontext = SDL_GL_CreateContext(gameWindow);

	if (glewInit())
	{
		//Unable to initialize GLEW ... exiting
		exit(EXIT_FAILURE);
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glFrontFace(GL_CCW);
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	modelManager = ModelManager::getInstance();

	currentLevel = nullptr;

	/* Get Moniter Refresh Rate */
	SDL_DisplayMode target, moniter;
	target.w = properties->getVideoProperties()->screenWidth;
	target.h = properties->getVideoProperties()->screenHeight;
	target.format = 0;  // don't care
	target.refresh_rate = 0; // don't care
	target.driverdata = 0; // initialize to 0
	SDL_GetClosestDisplayMode(0, &target, &moniter);
	FPS_CAP = moniter.refresh_rate;

	//FPS
	{
		timeSinceLastUpdate = timeStart = glutGet(GLUT_ELAPSED_TIME);
		updatesCount = 0;
		timeSinceLastFPSUpdate = timeStart;
		FPS = 0;
	}
};

Game::~Game()
{
	if (currentLevel)
		delete currentLevel;

	/* delete managers */
	delete FontManager::getInstance();

	/* delete SDL stuff */
	delete audioManager;
	delete modelManager;
	delete properties;

	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyRenderer(gameRenderer);
	SDL_DestroyWindow(gameWindow);

	Mix_Quit();
	TTF_Quit();
	SDLNet_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Game::StartGame()
{
	//Event Handling
	SDL_Event evt;

	Uint32 timeSinceLastRender = 0;

	/* Game Loop */
	while (isRunning)
	{
		timeSincelastUpdate = SDL_GetTicks() - lastUpdateTime;
		totalTime += timeSincelastUpdate;

		if (timeSincelastUpdate > 300.f)
			timeSincelastUpdate = 300.f;

		//if (timeSincelastUpdate >= 16)
		{
			lastUpdateTime = SDL_GetTicks();

			InputController::getInstance()->motionEvent = SDL_MouseMotionEvent();

			while (SDL_PollEvent(&evt))
			{
				//Pass events to Input Controller
				InputController::getInstance()->takeEvent(evt);

				if (evt.type == SDL_QUIT)
				{
					isRunning = false;
					printf("\nExit\n");
				}
			}
			SDL_PumpEvents();

			EngineUpdate(timeSincelastUpdate / 1000.0f);
			PhysicsWorld::getInstance()->Update(timeSincelastUpdate / 1000.0f);

			timeSinceLastRender += timeSincelastUpdate;

			//if (timeSinceLastRender >= 1000 / FPS_CAP)
			{
				timeSinceLastRender = 0;
				EngineRender();
			}

		}
	}
}

void Game::EngineUpdate(float _timeStep)
{
	//Load content of the new level.
	if (callInitOnNextUpdate)
	{
		callInitOnNextUpdate = false;
		
		if (currentLevel)
		{
			std::cout << "DELETING CURRENT LEVEL.... \n";
			delete currentLevel;
		}

		std::cout << "CLEARING COMPONENTS.... \n";
		RenderableComponent::renderableComponents.clear();
		std::cout << "UNLOADING MODELS.... \n";
		Models->UnloadModels();

		currentLevel = levelToLoad;
		levelToLoad = nullptr;

		//Make sure physics world is not paused
		PhysicsWorld::getInstance()->isPhysicsRunning = true;

		std::cout << "CALLING LEVEL INIT.... \n";
		currentLevel->init();

		std::cout << "\nLEVEL LOADED!\n\n";
	}

	if (currentLevel)
		currentLevel->LevelUpdate(_timeStep);

	Update(_timeStep);

	InputController::getInstance()->Update();
}

void Game::EngineRender()
{
	//FPS
	{
		int updateTime = glutGet(GLUT_ELAPSED_TIME);
		timeSinceLastUpdate = updateTime;
		updatesCount++;

		if (updatesCount >= 10)
		{
			FPS = updatesCount / ((float)(updateTime - timeSinceLastFPSUpdate) / 1000.f);
			timeSinceLastFPSUpdate = updateTime;
			updatesCount = 0;
		}
	}

	if (!callInitOnNextUpdate)
	{
		/* Set the background */
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		/* Clear The Screen And The Depth Buffer */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

		//Call the render for the current level
		if (currentLevel)
			currentLevel->LevelRender();

		//Call the game specific Render function
		Render();

		/* Render to screen */
		SDL_GL_SwapWindow(gameWindow);
	}
	PostRender();
}

void Game::PostRender()
{
	if (currentLevel)
		currentLevel->LevelCleanUp();
}

void Game::LoadLevel(Level * _level)
{
	if (levelToLoad != nullptr)
		delete levelToLoad;

	levelToLoad = _level;

	callInitOnNextUpdate = true;

	std::cout << "\nLOADING LEVEL...\n";
}

void Game::setRunning(bool _isRunning) {
	isRunning = _isRunning;
}

float Game::GetFPS()
{
	return FPS;
}

float Game::GetTotalTime()
{
	return totalTime / 1000.0f;
}