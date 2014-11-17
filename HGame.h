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
	int now_turn;
	
	HDeck *unknown_cards;
	HDeck *visible_cards[12]; // Save Cards as Month Order
	
	int  marker_stack[12]; // This stores duplicated cards' index at visible_cards
	int  marker_stack_size;
	bool was_nagari;      // Was Prev Game Nagari???
} HGame;

HGame *new_HGame(HCard const *CARD_SET);
void   delete_HGame(HGame *me);

void   HGame_reset(HGame *me, HCard const *CARD_SET); // This reset the level. But not reset every data.
void   HGame_setTurn(HGame *me, HPlayer *who_win); // Make winner first.

void   HGame_draw(HGame *me);

#endif