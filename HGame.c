#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include "HCard.h"
#include "HDeck.h"
#include "HPlayer.h"
#include "HGame.h"
#include "Debug_Center.h"

HGame *new_HGame(HCard *CARD_SET) // todo 
{
	HGame *me = (HGame *)malloc(sizeof(HGame));
#ifdef DEBUG
	if(me == NULL)
	{
		printError("HGame", "Error", "new_HGame(HCard *)", "Allocation Fail!!");
	}
	if(CARD_SET == NULL)
	{
		printError("HGame", "Error", "new_HGame(HCard *)", "No Card Set Available!!");
	}
#endif
	
	// Functino Assign
	me->reset = HGame_reset;
	me->setTurn = HGame_setTurn;
	
	// Allocate HPlayer & HDeck
	for(int i=0; i<3; ++i)
	{
		me->player[i] = new_HPlayer();
	}
	me->unknown_cards = new_HDeck();
	me->visible_cards = new_HDeck();
	
	me->marker_stack_size = 0;
	me->was_nagari = false;
	
	me->reset(me, CARD_SET);
}

void delete_HGame(HGame *me)
{
	// Deallocate HPlayer & HDeck
	for(int i=0; i<3; ++i)
	{
		delete_HPlayer(me->player[i]);
	}
	delete_HDeck(me->unknown_cards);
	delete_HDeck(me->visible_cards);
}

void HGame_reset(HGame *me, HCard *CARD_SET)
{
#ifdef DEBUG
	if(me == NULL)
	{
		printError("HGame", "Error", "reset(HGame *)", "NULL HGame Pointer Exception!!");
	}
#endif
	// Prepare for ingredient cards.
	me->unknown_cards->clear(me->unknown_cards);
	me->visible_cards->clear(me->visible_cards);
	for(int i=0; i<48; ++i)
	{
		me->unknown_cards->push(me->unknown_cards, &CARD_SET[i]);
	}
	me->unknown_cards->shake(me->unknown_cards);
	
	// Card Distribution. From Last Card.
	// Draw 3 cards on the floor
	for(int i=0; i<3; ++i)
	{
		me->visible_cards->drawFrom(me->visible_cards, me->unknown_cards, me->unknown_cards->size-1);
	}
	
	// Give Player the Cards
	for(int i=0; i<3; ++i)
	{
		for(int j=0; j<4; ++j) // Give 4 cards
		{
			me->player[i]->myDeck->drawFrom(me->player[i]->myDeck, me->unknown_cards, me->unknown_cards->size-1);
		}
	}
	
	// Draw 3 cards on the floor again.
	for(int i=0; i<3; ++i)
	{
		me->visible_cards->drawFrom(me->visible_cards, me->unknown_cards, me->unknown_cards->size-1);
	}
	
	// Give Player the Cards
	for(int i=0; i<3; ++i)
	{
		for(int j=0; j<3; ++j) // Give 4 cards
		{
			me->player[i]->myDeck->drawFrom(me->player[i]->myDeck, me->unknown_cards, me->unknown_cards->size-1);
		}
	}
}

void HGame_setTurn(HGame *me, HPlayer *who_win)
{
#ifdef DEBUG
	if(me == NULL)
	{
		printError("HGame", "Error", "setTurn(HGame *, HPlayer *)", "NULL HGame Pointer Exception!!");
	}
#endif
	// Search index
	int who = -1; // If nagari, no change will happen
	for(int i=0; i<3; ++i)
	{
		if(me->player[i] == who_win)
		{
			who = i;
			break;
		}
	}
	
	// Swap
	for(int i=who; i>0; --i)
	{
		HPlayer *temp   = me->player[i];
		me->player[i]   = me->player[i-1];
		me->player[i-1] = temp;
	}
}