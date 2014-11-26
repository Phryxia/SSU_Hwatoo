#ifndef HNOTICE_H
#define HNOTICE_H
#include "HPlayer.h"

/*
	ABOUT HNotice.h

	This is a simple GUI Module.
	They are top level module of the GUI hierarchy system.
	They use low level GUI : Renderer.h
*/

void HNotice_FClr(HPlayer const *player);

void HNotice_MBak(HPlayer const *player);
void HNotice_PBak(HPlayer const *player);
void HNotice_GBak(HPlayer const *player);

void HNotice_Win(HPlayer const *player);
void HNotice_Lose(HPlayer const *player, int money);
void HNotice_Nagari(void);
void HNotice_Bankrupt(HPlayer const *player);

#endif