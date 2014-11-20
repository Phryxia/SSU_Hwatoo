#ifndef RENDERER_H
#define RENDERER_H
#include <stdbool.h>
#include "HCard.h"
#include "HGame.h"

/*
	GAME
*/
void Renderer_card(int xpos, int ypos, HCard const *card);
void Renderer_cardsmall(int xpos, int ypos, HCard const *card);
void Renderer_deck(int xpos, int ypos, HDeck *deck, int marker, bool stack);
void Renderer_decksmall(int xpos, int ypos, HDeck *deck);
void Renderer_game(HGame *game, int selector);

/*
	DIALOGUE
*/
int  Renderer_eatw(HDeck *deck);
void Renderer_apChange(HPlayer *player);
void Renderer_notice(char const *contents, int height);
void Renderer_noticeCards(HDeck *deck);

/*
	INTRO
*/
void Renderer_intro(void);

#endif