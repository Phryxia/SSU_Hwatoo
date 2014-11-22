#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>
#include "HCard.h"
#include "HDeck.h"
#include "HPlayer.h"
#include "Debug_Center.h"

HPlayer *new_HPlayer(void)
{
	HPlayer *me = (HPlayer *)malloc(sizeof(HPlayer));

	if(me == NULL)
	{
 #ifdef DEBUG
		printError("HPlayer", "Error", "new_HPlayer(void)", "Allocation Fail!!");
 #endif
		return NULL;
	}
	else
	{
		/*
			Deck Data Reset
		*/
		me->myDeck      = new_HDeck();
		me->normDeck	= new_HDeck();
		me->animDeck	= new_HDeck();
		me->lineDeck	= new_HDeck();
		me->gwanDeck	= new_HDeck();

		HPlayer_init(me, true);
		
		return me;
	}
}

void delete_HPlayer(HPlayer *me)
{
	/*
		Delete Deck Data
	*/
	delete_HDeck(me->myDeck); // Because this contains several dynamic data.
	delete_HDeck(me->normDeck);
	delete_HDeck(me->animDeck);
	delete_HDeck(me->lineDeck);
	delete_HDeck(me->gwanDeck);
	free(me);
}

void HPlayer_init(HPlayer *me, bool money_reset)
{
	if(money_reset)
	{
		me->money       = 0;
	}

	HDeck_clear(me->myDeck);
	HDeck_clear(me->normDeck);
	HDeck_clear(me->animDeck);
	HDeck_clear(me->lineDeck);
	HDeck_clear(me->gwanDeck);
	
	me->how_many_go  = 0;
	me->score        = 0;
	me->score_lastgo = 0;
	me->hasChangeAP  = false;

	for(int m=0; m<12; ++m)
	{
		me->shaked[m] = false;
	}
}

void HPlayer_setName(HPlayer *me, char const *_name)
{
	if(me == NULL)
	{
 #ifdef DEBUG
		printError("HPlayer", "Error", "setName(HPlayer *, char const *)", "NULL HPlayer Pointer Exception!!");
 #endif
	}
	else
	{
		strncpy(me->name, _name, 31);
		me->name[31] = '\0';
	}
}

void HPlayer_eat(HPlayer *me, HCard const *card)
{
	if(me == NULL)
	{
 #ifdef DEBUG
		printError("HPlayer", "Error", "eat(HPlayer *, HCard *)", "NULL HPlayer Exception");
 #endif
	}
	else if(card == NULL)
	{
 #ifdef DEBUG
		printError("HPlayer", "Error", "eat(HPlayer *, HCard *)", "NULL HCard Exception");
 #endif
	}
	else
	{
		switch(card->type)
		{
			case H_NORM:
				HDeck_push(me->normDeck, card);
				break;
			case H_ANIM:
				// 9 Animal is default for Norm Card
				if(card->month == 9)
				{
					HDeck_push(me->normDeck, card);
				}
				else
				{
					HDeck_push(me->animDeck, card);
				}
				break;
			case H_LINE:
				HDeck_push(me->lineDeck, card);
				break;
			case H_GWAN:
				HDeck_push(me->gwanDeck, card);
				break;
		}
	}
}