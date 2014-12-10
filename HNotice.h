#ifndef HNOTICE_H
#define HNOTICE_H
#include "HPlayer.h"

/*
	ABOUT HNotice.h

	This is a simple GUI Module.
	They are top level module of the GUI hierarchy system.
	They use low level GUI : Renderer.h
*/

void HNotice_FClr(HPlayer const *player); // Pan Ssul E
void HNotice_Shake(HPlayer const *player);

void HNotice_MBak(HPlayer const *player); // Meong-tong-guri
void HNotice_PBak(HPlayer const *player); // Pi-Back
void HNotice_GBak(HPlayer const *player); // Gwang-Back

void HNotice_Win(HPlayer const *player);  // Win
void HNotice_Lose(HPlayer const *player, int money); // Lose
void HNotice_Nagari(void); // Nagari
void HNotice_President(HPlayer const *player);
void HNotice_Bankrupt(HPlayer const *player);

#endif