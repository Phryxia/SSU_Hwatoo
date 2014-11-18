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
		// Allocate HPlayer & HDeck
		for(int i=0; i<3; ++i)
		{
			(me->player)[i] = new_HPlayer();
		}
		me->unknown_cards = new_HDeck();
		for(int i=0; i<12; ++i)
		{
			(me->visible_cards)[i] = new_HDeck();
		}
		me->display_cards = new_HDeck();

		me->marker_stack_size = 0;
		me->was_nagari = false;

		HPlayer_setName((me->player)[0], "Min-Su");
		HPlayer_setName((me->player)[1], "Yeong-Heui");
		HPlayer_setName((me->player)[2], "Cheol-Su");

		HGame_reset(me, CARD_SET);
	}

	return me;
}

void delete_HGame(HGame *me)
{
	// Deallocate HPlayer & HDeck
	for(int i=0; i<3; ++i)
	{
		delete_HPlayer((me->player)[i]);
	}
	delete_HDeck(me->unknown_cards);
	for(int i=0; i<12; ++i)
	{
		delete_HDeck((me->visible_cards)[i]);
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
		HDeck_clear((me->visible_cards)[i]);
	}
	HDeck_clear(me->unknown_cards);
	HDeck_import(me->unknown_cards, CARD_SET);
	HDeck_shake(me->unknown_cards);
	
	// Card Distribution. From Last Card.
	// Draw 3 cards on the floor
	for(int i=0; i<3; ++i)
	{
		HCard const *topCard = me->unknown_cards->first->prev->data;
		HDeck_drawFrom((me->visible_cards)[topCard->month-1], me->unknown_cards, me->unknown_cards->size-1);
	}
	
	// Give Player the Cards
	for(int i=0; i<3; ++i)
	{
		HDeck_clear((me->player)[i]->myDeck);
		for(int j=0; j<4; ++j) // Give 4 cards
		{
			HDeck_drawFrom((me->player)[i]->myDeck, me->unknown_cards, me->unknown_cards->size-1);
		}
	}
	
	// Draw 3 cards on the floor again.
	for(int i=0; i<3; ++i)
	{
		HCard const *topCard = me->unknown_cards->first->prev->data;
		HDeck_drawFrom((me->visible_cards)[topCard->month-1], me->unknown_cards, me->unknown_cards->size-1);
	}
	
	// Give Player the Cards
	for(int i=0; i<3; ++i)
	{
		for(int j=0; j<3; ++j) // Give 4 cards
		{
			HDeck_drawFrom((me->player)[i]->myDeck, me->unknown_cards, me->unknown_cards->size-1);
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

void HGame_draw(HGame *me, int selector)
{
	if(me == NULL)
	{
 #ifdef DEBUG
		printError("HGame", "Error", "draw(HGame *)", "Cannot draw this game : NULL");
 #endif
		return ;
	}
	HGUI_erase();

	// Draw the Window
	for(int i=0; i<4; ++i)
	{
		HGUI_window(1, 1+10*i, 46, 10*(i+1)+2);
		
		// Grid Pattern
		for(int y=0; y<9; ++y)
		{
			for(int x=0; x<43; ++x)
			{
				if((x+y)%2 == 0)
				{
					HGUI_cSet(BLACK, BACKGROUND, DARK);
					HGUI_cSet(BLACK, FOREGROUND, BRIGHT);
				}
				else
				{
					HGUI_cSet(BLACK, BACKGROUND, DARK);
					HGUI_cSet(BLACK, FOREGROUND, DARK);
				}
				
				//HGUI_rect(2+x, 2+10*i+y, 3+x, 3+10*i+y);
				HGUI_text(x+2, 2 + 10*i + y, ".", false, ALIGN_LEFT);
			}
		}
		//HGUI_rect(2, 2+10*i, 45, 10*(i+1)+1);
		HGUI_cReset();
	}

	// Order List
	HGUI_text(3, 2, "ACTION : ", false, ALIGN_LEFT);

	// Draw Game Cards
	HGUI_text(3, 3, "GAME CARD AREA", false, ALIGN_LEFT);

	// Gather the Card from different slot
	int x_cnt = 0;
	int y_cnt = 0;
	HCard const *crnt_card = NULL;
	HCard const *prev_card = NULL;
	for(int c=0; c<(me->display_cards)->size; ++c)
	{
		crnt_card = HDeck_get(me->display_cards, c)->data;

		// Stack Same Month Card
		if(c > 0)
		{
			if(prev_card->month == crnt_card->month)
			{
				++y_cnt;
			}
			else
			{
				++x_cnt;
				y_cnt = 0;
			}
		}
		HGUI_card(3 + (CARD_WIDTH+1)*x_cnt, 5 - y_cnt, crnt_card);
		prev_card = crnt_card;
	}

	// Draw Player's Cards
	int pbias_y = 12;
	int pbias_x = 3;
	for(int i=0; i<3; ++i)
	{
		// Name
		HGUI_curSet(pbias_x, pbias_y+10*i+1);
		if(me->current_player_num == i)
		{
			// When Selected Turns
			HGUI_cSet(RED, BACKGROUND, DARK);
			HGUI_cSet(BLACK, FOREGROUND, DARK);
		}
		else
		{
			HGUI_cReset();
		}
		printf("PLAYER %d : %s", i+1, (me->player)[i]->name);
		
		// Score
		char score_string[32];
		sprintf(score_string, "Score : %3d", (me->player)[i]->score);
		HGUI_text(pbias_x + 16, pbias_y + 10*i +1, score_string, false, ALIGN_LEFT);
		HGUI_cReset();

		// Player's Deck Output
		for(int c=0; c<(me->player)[i]->myDeck->size; ++c)
		{
			HGUI_card(pbias_x + (CARD_WIDTH+1)*c, pbias_y+10*i+3, HDeck_get((me->player)[i]->myDeck, c)->data);
			HGUI_cSet(YELLOW, FOREGROUND, BRIGHT);
			if(selector == c && me->current_player_num == i)
			{
				HGUI_text(pbias_x + (CARD_WIDTH+1)*c + 0, pbias_y+10*i + 8, "^", false, ALIGN_LEFT);
				HGUI_text(pbias_x + (CARD_WIDTH+1)*c + 2, pbias_y+10*i + 8, "^", false, ALIGN_LEFT);
				HGUI_text(pbias_x + (CARD_WIDTH+1)*c + 4, pbias_y+10*i + 8, "^", false, ALIGN_LEFT);
			}
			else
			{
				HGUI_text(pbias_x + (CARD_WIDTH+1)*c + 0, pbias_y+10*i + 8, " ", false, ALIGN_LEFT);
				HGUI_text(pbias_x + (CARD_WIDTH+1)*c + 2, pbias_y+10*i + 8, " ", false, ALIGN_LEFT);
				HGUI_text(pbias_x + (CARD_WIDTH+1)*c + 4, pbias_y+10*i + 8, " ", false, ALIGN_LEFT);
			}
			HGUI_cReset();
		}
	}
}

void HGame_refresh(HGame *me)
{
	HDeck_clear(me->display_cards);
	for(int m=0; m<12; ++m)
	{
		// Gather from m th month
		for(int c=0; c<(me->visible_cards)[m]->size; ++c)
		{
			HDeck_push(me->display_cards, HDeck_get((me->visible_cards)[m], c)->data);
		}
	}
}

void HGame_initTurn(HGame *me)
{
	me->current_player_num = 0;
	me->current_player     = (me->player)[me->current_player_num];
}

void HGame_moveTurn(HGame *me)
{
	if(me->current_player_num < 2)
	{
		++(me->current_player_num);
	}
	else
	{
		me->current_player_num = 0;
	}
	me->current_player = (me->player)[me->current_player_num];
}