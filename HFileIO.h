#ifndef HFILEIO_H
#define HFILEIO_H
#include "HGame.h"

/*
	ABOUT HFILEIO

	HFileIO is a module to save & load the game.
	It is saved as "savedata.txt". (I don't know why they make us use text type...)
	This is implemented as Stack Structure.

	Return values of these functions are for annoucing success or failure.

	By technical issues, CARD_SET is needed. (So use this after using halloc)
*/
bool HFileIO_saveGame(HGame *game, HCard const *CARD_SET);
bool HFileIO_loadGame(HGame *game, HCard const *CARD_SET);

#endif