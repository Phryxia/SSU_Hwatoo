#ifndef HPLAYER_H
#define HPLAYER_H
#include <stdbool.h>
#include "HCard.h"
#include "HDeck.h"

/*
	HPlayer.h will provide Player's Information.
	
	Attribution by : Nho Je Seung
*/
typedef struct _HPlayer_
{
	/*
		General
	*/
	char   name[32];
	int    money;
	
	/*
		Game Play
	*/
	HDeck *myDeck;
	HDeck *normDeck;
	HDeck *animDeck;
	HDeck *lineDeck;
	HDeck *gwanDeck;
	
	int    how_many_go;
	int    score;

	bool hasChangeAP; // PPI -> Animal changed?

	/*
		UTIL
	*/
	char last_state[20]; // Character
} HPlayer;

HPlayer *new_HPlayer(void);       // Make a new HPlayer and return its pointer.
void delete_HPlayer(HPlayer *me); // When you finish using HPlayer, please deallocate this.

void HPlayer_setName(HPlayer *me, char const *_name);
void HPlayer_setState(HPlayer *me, char const *state);
void HPlayer_eat(HPlayer *me, HCard const *card);

#endif
