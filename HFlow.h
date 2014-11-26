#ifndef HFLOW_H
#define HFLOW_H

#include "HCard.h"
#include "HDeck.h"
#include "HPlayer.h"
#include "HGame.h"
#include "HGUI.h"
#include "Renderer.h"
#include "HNotice.h"
#include "HFileIO.h"
#include "Debug_Center.h"
#include <stdbool.h>

void HFlow_doGame(HGame *game, HCard const *CARD_SET, bool wasLoaded);
bool HFlow_eat(HGame *game, int card_pointer);
int  HFlow_isEnd(HGame *game);
void HFlow_stealcard(HGame *game);
bool HFlow_stop(HGame *game, int winner, bool hasGwan);

#endif