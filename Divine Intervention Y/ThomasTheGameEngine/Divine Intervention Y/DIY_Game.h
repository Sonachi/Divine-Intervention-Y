//Author:	Mathieu Violette
//Date:		2/08/2016

#ifndef DIY_GAME_H
#define DIY_GAME_H

#include <Game.h>


class DIY_Game : public Game
{
public:

	DIY_Game();
	~DIY_Game();


protected:

	/* Game Update Code Here */
	void Update(float _timestep);

	/* Game Render Code Here */
	void Render();
};

#endif