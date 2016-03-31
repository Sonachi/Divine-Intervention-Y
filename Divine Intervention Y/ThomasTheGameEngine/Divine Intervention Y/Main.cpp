//Author:	Mathieu Violette
//Date:		2/08/2016
//Divine Intervention Y

#include <conio.h>
#include "DIY_Game.h"

int main()
{
	DIY_Game* divineInterventionY_Game = new DIY_Game();
	divineInterventionY_Game->StartGame();
	delete divineInterventionY_Game;

	return 0;
}