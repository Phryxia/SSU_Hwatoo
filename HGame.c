#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <termios.h>
#include "HCard.h"
#include "HDeck.h"
#include "HPlayer.h"
#include "HGame.h"
#include "HGUI.h"
#include "Debug_Center.h"

HGame *new_HGame(HCard const *CARD_SET) // todo 
{
	HGame *me = (HGame *)malloc(sizeof(HGame));

	if(me == NULL)
	{
#ifdef DEBUG
		printError("HGame", "Error", "new_HGame(HCard *)", "Allocation Fail!!");
#endif
	}
	else if(CARD_SET == NULL)
	{
#ifdef DEBUG
		printError("HGame", "Error", "new_HGame(HCard *)", "No Card Set Available!!");
#endif
	}
	else
	{
		// Function Assign
		me->reset   = HGame_reset;
		me->setTurn = HGame_setTurn;
		me->draw    = HGame_draw;
		
		// Allocate HPlayer & HDeck
		for(int i=0; i<3; ++i)
		{
			me->player[i] = new_HPlayer();
		}
		me->unknown_cards = new_HDeck();
		for(int i=0; i<12; ++i)
		{
			me->visible_cards[i] = new_HDeck();
		}
		
		me->marker_stack_size = 0;
		me->was_nagari = false;

		me->player[0]->setName(me->player[0], "Min-Su");
		me->player[1]->setName(me->player[1], "Yeong-Heui");
		me->player[2]->setName(me->player[2], "Cheol-Su");

		me->reset(me, CARD_SET);
	}
}

void delete_HGame(HGame *me)
{
	// Deallocate HPlayer & HDeck
	for(int i=0; i<3; ++i)
	{
		delete_HPlayer(me->player[i]);
	}
	delete_HDeck(me->unknown_cards);
	for(int i=0; i<12; ++i)
	{
		delete_HDeck(me->visible_cards[i]);
	}
}

void HGame_reset(HGame *me, HCard const *CARD_SET)
{
#ifdef DEBUG
	if(me == NULL)
	{
		printError("HGame", "Error", "reset(HGame *, HCard *)", "NULL HGame Pointer Exception!!");
	}
	if(CARD_SET == NULL)
	{
		printError("HGame", "Error", "reset(HGame *, HCard *)", "NULL ingredient Exception!!");
	}
#endif
	// Prepare for ingredient cards.
	for(int i=0; i<12; ++i)
	{
		me->visible_cards[i]->klear(me->visible_cards[i]);
	}
	me->unknown_cards->klear(me->unknown_cards);
	me->unknown_cards->import(me->unknown_cards, CARD_SET);
	me->unknown_cards->shake(me->unknown_cards);
	
	// Card Distribution. From Last Card.
	// Draw 3 cards on the floor
	for(int i=0; i<3; ++i)
	{
		HCard const *topCard = me->unknown_cards->first->prev->data;
		me->visible_cards[topCard->month-1]->drawFrom(me->visible_cards[topCard->month-1],
													  me->unknown_cards, me->unknown_cards->size-1);
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
		HCard const *topCard = me->unknown_cards->first->prev->data;
		me->visible_cards[topCard->month-1]->drawFrom(me->visible_cards[topCard->month-1],
													  me->unknown_cards, me->unknown_cards->size-1);
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

void HGame_draw(HGame *me)
{
	if(me == NULL)
	{
#ifdef DEBUG
		printError("HGame", "Error", "draw(HGame *)", "Cannot draw this game : NULL");
#endif
		return ;
	}
	HGUI_erase();

	// Order List
	HGUI_curSet(2, 2);
	printf("ACTION : ");

	// Draw Game Cards
	HGUI_curSet(2, 3);
	printf("GAME CARD REGION");

	// Draw Player's Cards
	for(int i=0; i<3; ++i)
	{
		// Name
		HGUI_curSet(2, 8*(i+1));
		printf("PLAYER %d : %s", i+1, me->player[i]->name);
	}
}