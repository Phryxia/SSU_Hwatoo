#ifndef HGAME_H
#define HGAME_H
#include <stdbool.h>
#include "HCard.h"
#include "HDeck.h"
#include "HPlayer.h"

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

void   HGame_reset(HGame *me, HCard const *CARD_SET); // This reset the level. But not reset every data.
void   HGame_setTurn(HGame *me, HPlayer *who_win); // Make winner first.

void   HGame_draw(HGame *me, int selector);

/*
	ABOUT GAME SYSTEM
*/
HPlayer *HGame_nowPlayer(HGame *me);
void     HGame_refresh(HGame *me); // Re-Arrange Graphical
void     HGame_calcScore(HGame *me);

/*
	ABOUT PLAYER
*/
void HGame_initTurn(HGame *me);
void HGame_moveTurn(HGame *me);

#endif