#ifndef RENDERER_H
#define RENDERER_H
#include <stdbool.h>
#include "HCard.h"
#include "HDeck.h"
#include "HPlayer.h"
#include "HGame.h"

/*
	GAME PRIMITIVE
*/
void Renderer_card(int xpos, int ypos, HCard const *card);
void Renderer_cardsmall(int xpos, int ypos, HCard const *card);
void Renderer_deck(int xpos, int ypos, HDeck *deck, int marker, bool stack);
void Renderer_decksmall(int xpos, int ypos, HDeck *deck);
void Renderer_game(HGame *game, int selector);

/*
	DIALOGUE & EVENT
*/
void Renderer_notice(char const *contents, int height);

void Renderer_showBalance(HGame *game);
int  Renderer_eatw(HDeck *deck);
void Renderer_apChange(HPlayer *player);
void Renderer_shake(HGame *game);
bool Renderer_willGo(void);

void Renderer_noticeCards(HDeck *deck);
void Renderer_statistics(HGame *game);
void Renderer_help(void);
bool Renderer_exit(void);

void Renderer_intro(void);

#endif