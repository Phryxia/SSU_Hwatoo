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
		for(int p=0; p<3; ++p)
		{
			me->player[p] = new_HPlayer();
			me->player[p]->money = 100000; // 용돈
		}

		me->unknown_cards = new_HDeck();
		for(int m=0; m<12; ++m)
		{
			me->visible_cards[m] = new_HDeck();
		}
		me->display_cards = new_HDeck();

		me->was_nagari = false;

		HGame_reset(me, CARD_SET);

		// Name Set
		HPlayer_setName(me->player[0], "Min-Su");
		HPlayer_setName(me->player[1], "Yeong-Heui");
		HPlayer_setName(me->player[2], "Cheol-Su");
	}

	return me;
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
	
	/*
		SCENE RESET

		i)  Floor State
		ii) Player State
	*/
	// Floor State
	for(int m=0; m<12; ++m)
	{
		HDeck_clear(me->visible_cards[m]);
	}
	HDeck_clear(me->unknown_cards);
	HDeck_import(me->unknown_cards, CARD_SET);
	HDeck_shake(me->unknown_cards);

	// Player State
	for(int p=0; p<3; ++p)
	{
		HPlayer_init(me->player[p], false); // Don't Reset Money!
	}
	
	/*
		CARD DISTRIBUTION

		i)  Floor Cards
		ii) Player Cards
	*/
	// Floor
	for(int i=0; i<6; ++i)
	{
		HCard const *topCard = me->unknown_cards->first->prev->data;
		HDeck_drawFrom((me->visible_cards)[topCard->month-1], me->unknown_cards, me->unknown_cards->size - 1);
	}
	
	// Player
	for(int p=0; p<3; ++p)
	{
		for(int j=0; j<7; ++j) // Give 4 cards
		{
			HDeck_drawFrom((me->player)[p]->myDeck, me->unknown_cards, me->unknown_cards->size-1);
		}
		HDeck_sort(me->player[p]->myDeck);
	}

	// Turn Init
	HGame_initTurn(me);

 #ifdef DEBUG
	printError("HGame", "Note", "reset(HGame *, HCard *)", "Game Reset Done");
 #endif
}

/*
	ABOUT GAME SYSTEM
*/
HPlayer *HGame_nowPlayer(HGame *me)
{
	if(me == NULL)
	{
 #ifdef DEBUG
		printError("HGame", "Error", "nowPlayer(HGame *)", "NULL HGame Pointer Exception");
 #endif
		return NULL;
	}
	else
	{
		return (me->player)[me->current_player_num];
	}
}

int HGame_willShake(HGame *me)
{
	/*
		ABOUT WILL SHAKE

		willShake() will search for 'shak-able' cards.

		i)  There exists shakable cards. -> return number of shakable sets.
		ii) There is no shakable cards.  -> return 0
	*/

	HPlayer *player = HGame_nowPlayer(me);
	int counter  = 0;
	int same_num = 0;
	HCard const *prev_card = HDeck_get(player->myDeck, 0)->data;
	for(int c=1; c<player->myDeck->size; ++c)
	{
		// Search for evrery cards. Deck should be sorted correctly.
		HCard const *this_card = HDeck_get(player->myDeck, c)->data;

		if(prev_card->month == this_card->month && !player->shaked[this_card->month-1])
		{
			++counter;

			if(counter == 2)
			{
				++same_num;
				player->shaked[this_card->month-1] = true; // Protect Duplication
			}
		}
		else
		{
			counter = 0;
		}
	}

	return same_num;
}

void HGame_refresh(HGame *me)
{
	// Clear the Display Buffer
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

void HGame_calcScore(HGame *game) //점수산출함수
{
	/*
		Calculate Every Player's Score.

		Reason why not calculate only one is 'Middle Change'.
		For example, there could be the situation that someone took other's cards.
		So You have to change everyone's score whenever who's turn is.
	*/
	for(int p=0; p<3; ++p)
	{
    	//인자에 점수산출을 넣거나 점수계산
    	HPlayer *player = game->player[p];
    	int score_norm = 0; // PPI
		int score_anim = 0; // Animal
		int score_line = 0; // Ddi
		int score_gwan = 0; // Gwang
    	int xRate      = 1;

    	//피 점수 계산
    	/*
    		Normal Deck(PPI) Calculation

    		Count the number of cards. Be-careful with Double PPI.
    	*/
    	int norm_cnt   = player->normDeck->size;
    	int double_cnt = 0;
    	for(int c=0; c<norm_cnt; ++c)
    	{
    		HCard const *card = HDeck_get(player->normDeck, c)->data;

    		if(card->isDouble)
    		{
    			++double_cnt;
    		}
    	}
    	if(norm_cnt + double_cnt >= 10)
    	{
    		score_norm = norm_cnt + double_cnt - 9;
    	}
	
    	// 십 점수 계산
    	/*
    		Animal Deck Calculation

    		When anim_cnt >= 5, add 1 point.
    		When godori apeared, 
    	*/
    	int anim_cnt = player->animDeck->size;
    	int bird_cnt = 0;
    	for(int c=0; c<anim_cnt; ++c)
    	{
    		// Search for Godori
    		HCard const *card = HDeck_get(player->animDeck, c)->data;

    		if(HCard_isBird(card))
    		{
    			++bird_cnt;

    			if(bird_cnt == 3)
    			{
    				break;
    			}
    		}
    	}
    	if(anim_cnt >= 5)
    	{
    		score_anim += anim_cnt - 4;

    		if(anim_cnt >= 7)
    		{
    			// Meong-Teong-Gu-Ri
    			xRate *= 2;
    		}
    	}
    	if(bird_cnt == 3)
    	{
    		score_anim += 5;
    	}
	
    	//띠 점수 계산
    	/*
    		Line Deck Calculation

    		First, just count the number of lines.
    		If line_cnt == 5, add 1 point.

    		And then, calculate each types for 3 point.
    	*/
    	int line_cnt = player->lineDeck->size;
    	int red_cnt  = 0;
    	int blu_cnt  = 0;
    	int cho_cnt  = 0;
    	for(int c=0; c<line_cnt; ++c)
    	{
    		HCard const *card = HDeck_get(player->lineDeck, c)->data;

    		switch(card->five_type)
    		{
    			case HF_RED:
    				++red_cnt;
    				break;
    			case HF_BLU:
    				++blu_cnt;
    				break;
    			case HF_CHO:
    				++cho_cnt;
    				break;
    		}
    	}
    	if(line_cnt >= 5)
    	{
    		score_line += line_cnt - 4;
    	}
    	if(red_cnt == 3) // Hong-Dan
    	{
    		score_line += 3;
    	}
    	if(blu_cnt == 3) // Cheong-Dan
    	{
    		score_line += 3;
    	}
    	if(cho_cnt == 3) // Cho-Dan
    	{
    		score_line += 3;
    	}

    	//광점수 계산
    	/*
    		Gwang Deck Calculation
    	*/
    	int  gwan_cnt   = player->gwanDeck->size;
    	bool has_bigwan = false;
    	for(int c=0; c<gwan_cnt; ++c)
    	{
    		// Search for Bi Gwan
    		HCard const *card = HDeck_get(player->gwanDeck, c)->data;

    		if(card->month == 12)
    		{
    			has_bigwan = true;
    			break;
    		}
    	}
    	switch(gwan_cnt)
    	{
    		case 5: // Wow!
    			score_gwan = 15;
    			break;
    		case 4:
    			score_gwan = 4;
    			break;
    		case 3:
    			if(has_bigwan) // Bi Gwang Calculation
    			{
    				score_gwan = 2;
    			}
    			else
    			{
    				score_gwan = 3;
    			}
    			break;
    	}

    	/*
    		Total Calculation
    	*/
    	player->score = xRate*(score_norm + score_anim + score_line + score_gwan);
	}
}

int  HGame_isPres(HGame *game, int *who)
{
	/*
		ABOUT CHONG-TONG
		Chong-tong should exist only one.
		If there are more than 2 chong-tong, game will be nagari.
		To examine whether chong-tong or not, this iterate every cards they have.
	*/
	// Chong-Tong Examination
	int how_many_pres = 0;
	int pres_who      = -1;
	for(int p=0; p<3; ++p)
	{
		// Examine every player's cards
		HPlayer *player = game->player[p];
		HCard const *prev_card = HDeck_get(player->myDeck, 0)->data;
		HCard const *this_card = NULL;
		int same_count = 0;
		
		for(int c=1; c<player->myDeck->size; ++c)
		{
			// Compare with previous card
			this_card = HDeck_get(player->myDeck, c)->data;
			if(prev_card->month == this_card->month)
			{
				++same_count;
				if(same_count == 3)
				{
					++how_many_pres;
					pres_who = p; // Assign Current Player as PResident
					break;
				}
			}
			else
			{
				// Reset when new card appears
				same_count = 0;
			}
			// Shift Card
			prev_card = this_card;
		}
	}

	if(how_many_pres == 1)
	{
		*who = pres_who;
		return 1; // Chong-Tong
	}
	else if(how_many_pres > 1)
	{
		return -1; // Nagari
	}
	else
	{
		return 0;
	}
}

/*
	ABOUT PLAYER
*/
void HGame_initTurn(HGame *me)
{
	me->current_player_num = 0;

 #ifdef DEBUG
	printError("HGame", "Note", "initTurn(HGame *)", "Turn Init Done");
 #endif
}

void HGame_setTurn(HGame *me, int who)
{
	// Swap
	for(int i=who; i>0; --i)
	{
		HPlayer *temp   = me->player[i];
		me->player[i]   = me->player[i-1];
		me->player[i-1] = temp;
	}
}