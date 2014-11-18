#include <stdio.h>
#include <malloc.h>
#include <string.h>
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
		me->name[0]     = '\0';
		me->money       = 0;
		me->myDeck      = new_HDeck();
		me->normDeck	= new_HDeck();
		me->animDeck	= new_HDeck();
		me->lineDeck	= new_HDeck();
		me->gwanDeck	= new_HDeck();
		
		me->how_many_go = 0;
		me->score       = 0;
		
		me->score_norm  = 0;
		me->score_anim  = 0;
		me->score_line  = 0;
		me->score_gwan  = 0;
		
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
				HDeck_push(me->animDeck, card);
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