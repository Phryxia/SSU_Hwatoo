#ifndef HGAME_H
#define HGAME_H
#include <stdbool.h>
#include "HCard.h"
#include "HDeck.h"
#include "HPlayer.h"

#define WON_PER_POINT 100
#define WIN_BY_GWAN 0

/*
	HGame.h handle the main game.
	They store some cards and players.
	
	Constructed by ; Kwon Se Kyu
*/
typedef struct _HGame_
{
	/*
		Member Variable
	*/
	HPlayer *player[3];
	int current_player_num;
	
	HDeck *unknown_cards;
	HDeck *visible_cards[12]; // Save Cards as Month Order
	HDeck *display_cards;

	bool was_nagari;      // Was Prev Game Nagari???

	/*
		GUI Factor
	*/
} HGame;

HGame *new_HGame(HCard const *CARD_SET);
void   delete_HGame(HGame *me);

void   HGame_reset(HGame *me, HCard const *CARD_SET); // This reset the level. But not reset money data.
void   HGame_draw(HGame *me, int selector);

/*
	ABOUT GAME SYSTEM
*/
HPlayer *HGame_nowPlayer(HGame *me);
int      HGame_willShake(HGame *me); // Return 
void     HGame_refresh(HGame *me); // Re-Arrange Graphical
void     HGame_calcScore(HGame *me);
int      HGame_isPres(HGame *game, int *who);

/*
	ABOUT PLAYER
*/
void HGame_initTurn(HGame *me);
void HGame_setTurn(HGame *me, int who);    // Make winner first.

#endif