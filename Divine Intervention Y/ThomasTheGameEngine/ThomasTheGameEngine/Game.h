//Author:	Mathieu Violette
//Date:		5/26/2015(MV), 6/6/2015(MV), 10/31/2015(NS)


/* 
	This is the core game, it holds the main loop, handles events
	and renders and draws the game.
*/

#ifndef _GAME_H_
#define _GAME_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_net.h>

#undef main
#include <glew.h>
#include <stdio.h>


#define GAME Game::GetInstance() 

class Level;		class GameProperties;	class InputController;
class AudioManager; class ModelManager;		

class Game
{
	friend class GameObject;

private:

	//Flag boolean that keeps the mainloop running or exits it
	bool isRunning;

	bool callInitOnNextUpdate;

	//variables to track gameTime
	Uint32 lastUpdateTime, timeSincelastUpdate, totalTime;
protected:	
	//SDL relevant variables to initialize the window
	SDL_Window* gameWindow;
	SDL_Renderer* gameRenderer;
	SDL_GLContext glcontext;
	
	float FPS;
	Level *levelToLoad;
	Game();	

	//Singleton instance
	static Game* instance;
public:
	//Pointers to non optional engine functionalities like resource
	//managers and the game properties
	GameProperties * properties;
	AudioManager * audioManager;
	ModelManager * modelManager;
	Level * currentLevel;
	InputController * inputManager;
	
	/* Returns the instance of Game */
	static Game* GetInstance()
	{
		if (instance == nullptr)
			instance = new Game();

		return instance;
	}

	~Game();
	
	/* Called to start game loop.*/
	void StartGame();

	/* Performs engine specific updates, then calls game's update function */
	void EngineUpdate(float timeStep); /* Engine's Update*/

	/* Performs engine draw calls, then calls game's render function */
	void EngineRender(); /* Engine's Render*/

	/* Does all the clean up of flagged gameObjects */
	void PostRender();

	//Functions that will be changed depending on the games specific needs
	virtual void Update(float timeStep){}
	virtual void Render(){}

	//Will load a new level into the engine & will unload the current level (if it exists)
	void LoadLevel(Level * _level);

	void setRunning(bool);

	float GetFPS();

	float GetTotalTime();
};

#endif