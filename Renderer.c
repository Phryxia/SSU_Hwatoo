#include <string.h>
#include "HCard.h"
#include "HDeck.h"
#include "HPlayer.h"
#include "HGame.h"
#include "HGUI.h"
#include "Renderer.h"
#include "Debug_Center.h"

void Renderer_card(int xpos, int ypos, HCard const *card)
{
	if(card == NULL)
	{
 #ifdef DEBUG
		printError("HGUI", "Error", "HGUI_card", "NULL Card Exception");
 #endif
	}

	char month_string[3] = {0};
	sprintf(month_string, "%d", card->month);
	switch(card->month)
	{
		case 1:
			HGUI_cSet(WHITE, BACKGROUND, BRIGHT);
			HGUI_rect(xpos, ypos, xpos+5, ypos+5);

			HGUI_cSet(BLACK, BACKGROUND, DARK);
			HGUI_rect(xpos  , ypos+3, xpos+1, ypos+5);
			HGUI_rect(xpos+1, ypos+2, xpos+2, ypos+5);
			HGUI_rect(xpos+2, ypos+4, xpos+3, ypos+5);
			HGUI_rect(xpos+3, ypos+3, xpos+4, ypos+5);
			HGUI_rect(xpos+4, ypos+2, xpos+5, ypos+5);

			HGUI_cSet(WHITE, FOREGROUND, DARK);
			HGUI_text(xpos  , ypos+3, "^", false, ALIGN_LEFT);
			HGUI_text(xpos+1, ypos+2, "^", false, ALIGN_LEFT);
			HGUI_text(xpos+2, ypos+4, "^", false, ALIGN_LEFT);
			HGUI_text(xpos+3, ypos+3, "^", false, ALIGN_LEFT);
			HGUI_text(xpos+4, ypos+2, "^", false, ALIGN_LEFT);

			if(card->type == H_GWAN)
			{
				HGUI_cSet(WHITE, FOREGROUND, BRIGHT);
				HGUI_text(xpos, ypos+4, "*", false, ALIGN_LEFT);
			}
			break;
		case 2:
			HGUI_cSet(WHITE, BACKGROUND, BRIGHT);
			HGUI_rect(xpos, ypos, xpos+5, ypos+5);

			HGUI_cSet(RED, BACKGROUND, BRIGHT);
			HGUI_rect(xpos, ypos+3, xpos+3, ypos+4);
			HGUI_rect(xpos+1, ypos+2, xpos+2, ypos+5);
			HGUI_rect(xpos+3, ypos+4, xpos+5, ypos+5);
			HGUI_rect(xpos+4, ypos+3, xpos+5, ypos+4);

			HGUI_cSet(YELLOW, BACKGROUND, BRIGHT);
			HGUI_rect(xpos+1, ypos+3, xpos+2, ypos+4);
			HGUI_rect(xpos+4, ypos+4, xpos+5, ypos+5);
			break;
		case 3:
			HGUI_cSet(WHITE, BACKGROUND, BRIGHT);
			HGUI_rect(xpos, ypos, xpos+5, ypos+5);

			if(card->type == H_GWAN)
			{
				HGUI_cSet(BLACK, FOREGROUND, DARK);
				HGUI_text(xpos, ypos+4, "*", false, ALIGN_LEFT);

			}

			HGUI_cSet(MAGENTA, BACKGROUND, BRIGHT);
			HGUI_rect(xpos, ypos+3, xpos+3, ypos+4);
			HGUI_rect(xpos+1, ypos+2, xpos+2, ypos+5);
			HGUI_rect(xpos+3, ypos+4, xpos+5, ypos+5);
			HGUI_rect(xpos+4, ypos+3, xpos+5, ypos+4);

			HGUI_cSet(MAGENTA, BACKGROUND, DARK);
			HGUI_rect(xpos+1, ypos+3, xpos+2, ypos+4);
			HGUI_rect(xpos+4, ypos+4, xpos+5, ypos+5);
			
			break;
		case 4:
			HGUI_cSet(WHITE, BACKGROUND, BRIGHT);
			HGUI_rect(xpos, ypos, xpos+5, ypos+5);

			HGUI_cSet(BLACK, FOREGROUND, DARK);
			HGUI_text(xpos+1, ypos+1, "|", false, ALIGN_LEFT);
			HGUI_text(xpos+1, ypos+2, "|", false, ALIGN_LEFT);
			HGUI_text(xpos+1, ypos+3, "|", false, ALIGN_LEFT);
			HGUI_text(xpos+1, ypos+4, "|", false, ALIGN_LEFT);
			HGUI_text(xpos+3, ypos+3, "|", false, ALIGN_LEFT);
			HGUI_text(xpos+3, ypos+4, "|", false, ALIGN_LEFT);
			HGUI_text(xpos  , ypos+1, "o", false, ALIGN_LEFT);
			HGUI_text(xpos  , ypos+3, "o", false, ALIGN_LEFT);
			HGUI_text(xpos+2, ypos+2, "o", false, ALIGN_LEFT);
			HGUI_text(xpos+2, ypos+4, "o", false, ALIGN_LEFT);
			HGUI_text(xpos+4, ypos+3, "o", false, ALIGN_LEFT);
			break;
		case 5:
			HGUI_cSet(WHITE, BACKGROUND, BRIGHT);
			HGUI_rect(xpos, ypos, xpos+5, ypos+5);

			if(card->type == H_ANIM)
			{
				HGUI_cSet(BLUE, BACKGROUND, BRIGHT);
				HGUI_cSet(YELLOW, FOREGROUND, BRIGHT);
				HGUI_text(xpos, ypos+4, "x xx", false, ALIGN_LEFT);
			}
			
			HGUI_cSet(WHITE, BACKGROUND, BRIGHT);
			HGUI_cSet(BLACK, FOREGROUND, DARK);
			HGUI_text(xpos+1, ypos+1, "|", false, ALIGN_LEFT); // Stem 1
			HGUI_text(xpos+1, ypos+2, "|", false, ALIGN_LEFT);
			HGUI_text(xpos+1, ypos+3, "|", false, ALIGN_LEFT);
			HGUI_text(xpos+1, ypos+4, "|", false, ALIGN_LEFT);
			HGUI_text(xpos+3, ypos+4, "|", false, ALIGN_LEFT); // Stem 2

			HGUI_cSet(BLUE, BACKGROUND, DARK);
			HGUI_cSet(BLUE, FOREGROUND, BRIGHT);
			HGUI_text(xpos  , ypos+2, "<", false, ALIGN_LEFT); // Flower 1
			HGUI_text(xpos+2, ypos+2, ">", false, ALIGN_LEFT);
			HGUI_text(xpos+1, ypos+1, "^", false, ALIGN_LEFT);
			HGUI_text(xpos+1, ypos+3, "o", false, ALIGN_LEFT);

			HGUI_cSet(WHITE, BACKGROUND, BRIGHT);
			HGUI_text(xpos+3, ypos+3, "x", false, ALIGN_LEFT); // Flower 2

			HGUI_cSet(YELLOW, BACKGROUND, BRIGHT);
			HGUI_text(xpos+1, ypos+2, " ", false, ALIGN_LEFT);
			break;
		case 6:
			HGUI_cSet(WHITE, BACKGROUND, BRIGHT);
			HGUI_rect(xpos, ypos, xpos+5, ypos+5);
			if(card->type == H_ANIM)
			{
				HGUI_text(xpos+3, ypos+2, ">", false, ALIGN_LEFT);
			} // Butter Fly Center

			HGUI_cSet(RED  , BACKGROUND, DARK);
			HGUI_cSet(YELLOW, FOREGROUND, DARK);
			HGUI_rect(xpos+1, ypos+2, xpos+3, ypos+4);
			if(card->type == H_ANIM)
			{
				HGUI_text(xpos+1, ypos+2, "<", false, ALIGN_LEFT); // Butter Fly Left
				HGUI_text(xpos+2, ypos+3, "|", false, ALIGN_LEFT);
			} // Butter Fly Center

			HGUI_cSet(RED  , BACKGROUND, BRIGHT);
			HGUI_rect(xpos+1, ypos+3, xpos+2, ypos+4);
			HGUI_rect(xpos+2, ypos+2, xpos+3, ypos+3);
			if(card->type == H_ANIM)
			{
				HGUI_text(xpos+2, ypos+2, "#", false, ALIGN_LEFT); // Butter Fly Center
			} // Butter Fly Center

			HGUI_cSet(BLACK, BACKGROUND, DARK);
			HGUI_cSet(BLACK, FOREGROUND, BRIGHT);
			HGUI_text(xpos, ypos+1, "\\", false, ALIGN_LEFT);
			HGUI_text(xpos, ypos+4, "/", false, ALIGN_LEFT);
			HGUI_text(xpos+3, ypos+1, "/", false, ALIGN_LEFT);
			HGUI_text(xpos+3, ypos+4, "\\", false, ALIGN_LEFT);
			break;
		case 7:
			HGUI_cSet(WHITE, BACKGROUND, BRIGHT);
			HGUI_rect(xpos, ypos, xpos+5, ypos+5);

			HGUI_cSet(BLACK, FOREGROUND, DARK);
			HGUI_text(xpos+1, ypos+1, "|", false, ALIGN_LEFT);
			HGUI_text(xpos+1, ypos+2, "|", false, ALIGN_LEFT);
			HGUI_text(xpos+1, ypos+3, "|", false, ALIGN_LEFT);
			HGUI_text(xpos+1, ypos+4, "|", false, ALIGN_LEFT);
			HGUI_text(xpos+3, ypos+3, "|", false, ALIGN_LEFT);
			HGUI_text(xpos+3, ypos+4, "|", false, ALIGN_LEFT);
			HGUI_text(xpos  , ypos+1, "o", false, ALIGN_LEFT);
			HGUI_text(xpos+2, ypos+4, "o", false, ALIGN_LEFT);

			HGUI_cSet(RED   , FOREGROUND, BRIGHT);
			HGUI_text(xpos  , ypos+3, "o", false, ALIGN_LEFT);
			HGUI_text(xpos+2, ypos+2, "o", false, ALIGN_LEFT);
			HGUI_text(xpos+4, ypos+3, "o", false, ALIGN_LEFT);
			break;
		case 8:
			if(card->type != H_GWAN)
			{
				HGUI_cSet(WHITE, BACKGROUND, BRIGHT);
			}
			else
			{
				HGUI_cSet(RED, BACKGROUND, DARK);
			}
			HGUI_rect(xpos, ypos, xpos+5, ypos+3);

			HGUI_cSet(WHITE, FOREGROUND, BRIGHT);
			HGUI_text(xpos+1, ypos+1, "+", false, ALIGN_LEFT);
			HGUI_text(xpos+2, ypos+1, "-", false, ALIGN_LEFT);
			HGUI_text(xpos+3, ypos+1, "+", false, ALIGN_LEFT);
			HGUI_text(xpos+1, ypos+2, "+", false, ALIGN_LEFT);
			HGUI_text(xpos+2, ypos+2, "-", false, ALIGN_LEFT);
			HGUI_text(xpos+3, ypos+2, "+", false, ALIGN_LEFT);

			HGUI_cSet(BLACK, BACKGROUND, DARK);
			HGUI_rect(xpos, ypos+3, xpos+5, ypos+5);
			
			if(card->type == H_GWAN)
			{
				HGUI_text(xpos, ypos+4, "*", false, ALIGN_LEFT);
			}
			break;
		case 9:
			HGUI_cSet(WHITE, BACKGROUND, BRIGHT);
			HGUI_rect(xpos, ypos, xpos+5, ypos+5);

			if(card->type == H_ANIM)
			{
				HGUI_cSet(BLUE, BACKGROUND, BRIGHT);
				HGUI_cSet(BLUE, FOREGROUND, DARK);
				HGUI_text(xpos, ypos+4, "~", false, ALIGN_LEFT);
				HGUI_text(xpos+1, ypos+4, "~", false, ALIGN_LEFT);
				HGUI_text(xpos+2, ypos+4, "~", false, ALIGN_LEFT);
				HGUI_text(xpos+3, ypos+4, "~", false, ALIGN_LEFT);
				HGUI_text(xpos+4, ypos+4, "~", false, ALIGN_LEFT);
				HGUI_text(xpos, ypos+3, "~", false, ALIGN_LEFT);
				HGUI_text(xpos+1, ypos+3, "~", false, ALIGN_LEFT);
				HGUI_text(xpos+2, ypos+3, "~", false, ALIGN_LEFT);
				HGUI_text(xpos+3, ypos+3, "~", false, ALIGN_LEFT);
				HGUI_text(xpos+4, ypos+3, "~", false, ALIGN_LEFT);
			}

			HGUI_cSet(BLACK, BACKGROUND, DARK);
			HGUI_cSet(BLACK, FOREGROUND, BRIGHT);
			HGUI_text(xpos  , ypos+4, "o", false, ALIGN_LEFT);
			HGUI_text(xpos+3, ypos+3, "o", false, ALIGN_LEFT);
			HGUI_cSet(WHITE, BACKGROUND, BRIGHT);
			HGUI_text(xpos+1, ypos+2, "|", false, ALIGN_LEFT);
			
			
			HGUI_cSet(YELLOW, BACKGROUND, BRIGHT);
			HGUI_cSet(RED, FOREGROUND, DARK);
			HGUI_text(xpos+1, ypos+4, "*", false, ALIGN_LEFT);
			HGUI_text(xpos+2, ypos+3, "*", false, ALIGN_LEFT);
			HGUI_text(xpos+1, ypos+1, "*", false, ALIGN_LEFT);
			HGUI_cSet(RED, BACKGROUND, BRIGHT);
			HGUI_cSet(YELLOW, FOREGROUND, DARK);
			HGUI_text(xpos+1, ypos+3, "*", false, ALIGN_LEFT);
			HGUI_text(xpos+2, ypos+4, "*", false, ALIGN_LEFT);
			HGUI_text(xpos+2, ypos+1, "*", false, ALIGN_LEFT);
			break;
		case 10:
			HGUI_cSet(WHITE, BACKGROUND, BRIGHT);
			HGUI_rect(xpos, ypos, xpos+5, ypos+5);

			HGUI_cSet(RED, FOREGROUND, BRIGHT);
			HGUI_text(xpos+0, ypos+3, "*", false, ALIGN_LEFT);
			HGUI_text(xpos+1, ypos+2, "*", false, ALIGN_LEFT);
			HGUI_text(xpos+3, ypos+4, "*", false, ALIGN_LEFT);

			HGUI_cSet(YELLOW, FOREGROUND, DARK);
			HGUI_text(xpos+2, ypos+3, "*", false, ALIGN_LEFT);
			HGUI_text(xpos+0, ypos+1, "*", false, ALIGN_LEFT);
			HGUI_text(xpos+4, ypos+2, "*", false, ALIGN_LEFT);

			HGUI_cSet(RED, FOREGROUND, DARK);
			HGUI_text(xpos+1, ypos+4, "*", false, ALIGN_LEFT);
			HGUI_text(xpos+4, ypos+3, "*", false, ALIGN_LEFT);
			HGUI_text(xpos+2, ypos+2, "*", false, ALIGN_LEFT);

			if(card->type == H_ANIM) // Draw Dear
			{
				HGUI_cSet(WHITE, BACKGROUND, BRIGHT);
				HGUI_cSet(YELLOW, FOREGROUND, DARK);
				HGUI_text(xpos, ypos+2, "<", false, ALIGN_LEFT); // Nose
				HGUI_cSet(YELLOW, BACKGROUND, DARK);
				HGUI_rect(xpos+1, ypos+3, xpos+4, ypos+4); // Body
				HGUI_rect(xpos+1, ypos+2, xpos+2, ypos+3); // Head
				HGUI_cSet(BLACK, FOREGROUND, DARK);
				HGUI_text(xpos+1, ypos+2, ".", false, ALIGN_LEFT); // Eye
				HGUI_cSet(WHITE, BACKGROUND, BRIGHT);
				HGUI_cSet(YELLOW, FOREGROUND, DARK);
				HGUI_text(xpos+1, ypos+4, "| |", false, ALIGN_LEFT); // LEG
			}
			break;
		case 11:
			if(card->isDouble)
			{
				HGUI_cSet(RED, BACKGROUND, DARK);
				HGUI_rect(xpos, ypos, xpos+5, ypos+5);
				HGUI_cSet(WHITE, BACKGROUND, BRIGHT);
				HGUI_rect(xpos, ypos, xpos+5, ypos+2);
			}
			else
			{
				HGUI_cSet(WHITE, BACKGROUND, BRIGHT);
				HGUI_rect(xpos, ypos, xpos+5, ypos+5);
			}
			

			HGUI_cSet(BLACK, FOREGROUND, DARK);
			if(card->type == H_GWAN)
			{
				HGUI_text(xpos, ypos+4, "*", false, ALIGN_LEFT);
			}
			HGUI_text(xpos+1, ypos+1, "|", false, ALIGN_LEFT); // Stem1
			if(card->isDouble)
			{
				HGUI_cSet(RED, BACKGROUND, DARK);
			}
			HGUI_text(xpos+1, ypos+2, "|", false, ALIGN_LEFT);
			HGUI_text(xpos+3, ypos+2, "|", false, ALIGN_LEFT); // Stem2
			HGUI_cSet(WHITE, BACKGROUND, BRIGHT);
			HGUI_cSet(BLUE  , FOREGROUND, BRIGHT);
			HGUI_text(xpos+2, ypos+1, "*", false, ALIGN_LEFT);
			if(card->isDouble)
			{
				HGUI_cSet(RED, BACKGROUND, DARK);
			}
			HGUI_text(xpos  , ypos+2, "*", false, ALIGN_LEFT);
			

			HGUI_cSet(BLACK, BACKGROUND, DARK); // Poo
			HGUI_cSet(BLACK, FOREGROUND, BRIGHT);
			HGUI_text(xpos+1, ypos+2, "^", false, ALIGN_LEFT);
			HGUI_text(xpos+0, ypos+3, "<", false, ALIGN_LEFT);
			HGUI_text(xpos+1, ypos+3, "^", false, ALIGN_LEFT);
			HGUI_text(xpos+2, ypos+3, "^", false, ALIGN_LEFT);
			HGUI_text(xpos+3, ypos+3, ">", false, ALIGN_LEFT);
			HGUI_text(xpos+2, ypos+4, "v", false, ALIGN_LEFT);
			break;
		case 12:
			HGUI_cSet(WHITE, BACKGROUND, BRIGHT);
			HGUI_rect(xpos, ypos, xpos+5, ypos+5);

			if(card->type == H_GWAN)
			{
				HGUI_cSet(RED, FOREGROUND, BRIGHT);
				HGUI_text(xpos+2, ypos+2, "o", false, ALIGN_LEFT);
				HGUI_text(xpos+2, ypos+3, "T", false, ALIGN_LEFT);
				HGUI_text(xpos+2, ypos+4, "^", false, ALIGN_LEFT);
			}

			HGUI_cSet(BLACK, FOREGROUND, BRIGHT);
			HGUI_text(xpos+0, ypos+2, "\\", false, ALIGN_LEFT); // Rain
			HGUI_text(xpos+1, ypos+1, "\\", false, ALIGN_LEFT);
			HGUI_text(xpos+3, ypos+2, "/", false, ALIGN_LEFT);
			HGUI_text(xpos+4, ypos+3, "/", false, ALIGN_LEFT);

			HGUI_cSet(BLACK, BACKGROUND, DARK);
			HGUI_text(xpos+0, ypos+0, "O", false, ALIGN_LEFT); // Cloud Left
			if(card->type == H_GWAN)
			{
				HGUI_cSet(WHITE, FOREGROUND, BRIGHT);
				HGUI_text(xpos+0, ypos+1, "*", false, ALIGN_LEFT);
				HGUI_cSet(BLACK, FOREGROUND, BRIGHT);
			}
			else
			{
				HGUI_text(xpos+0, ypos+1, "O", false, ALIGN_LEFT);
			}
			HGUI_text(xpos+1, ypos+0, "O", false, ALIGN_LEFT);
			HGUI_text(xpos+3, ypos+0, "O", false, ALIGN_LEFT); // Cloud Right
			HGUI_text(xpos+3, ypos+1, "O", false, ALIGN_LEFT);
			HGUI_text(xpos+4, ypos+0, "O", false, ALIGN_LEFT);
			HGUI_text(xpos+4, ypos+1, "O", false, ALIGN_LEFT);
			HGUI_text(xpos+4, ypos+2, "O", false, ALIGN_LEFT);

			break;
	}
	if(card->type == H_GWAN)
	{
		HGUI_cSet(BLACK, BACKGROUND, DARK);
		HGUI_cSet(WHITE, FOREGROUND, BRIGHT);
		HGUI_text(xpos, ypos, "GWAN", false, ALIGN_LEFT);
	}
	HGUI_cSet(BLACK, BACKGROUND, DARK);
	HGUI_cSet(RED  , FOREGROUND, BRIGHT);
	HGUI_text(xpos+4, ypos+4, month_string, false, ALIGN_RIGHT);
	HGUI_cReset();

	// Draw DDII
	//HGUI_rect(xpos, ypos, xpos+5, ypos+5);
	if(card->type == H_LINE)
	{
		switch(card->five_type)
		{
			case HF_RED:
				HGUI_cSet(RED, BACKGROUND, DARK);
				HGUI_rect(xpos+3, ypos, xpos+4, ypos+3);
				HGUI_cSet(WHITE, FOREGROUND, BRIGHT);
				HGUI_text(xpos+3, ypos, "R", false, ALIGN_LEFT);
				HGUI_text(xpos+3, ypos+1, "E", false, ALIGN_LEFT);
				HGUI_text(xpos+3, ypos+2, "D", false, ALIGN_LEFT);
				break;
			case HF_BLU:
				HGUI_cSet(BLUE, BACKGROUND, BRIGHT);
				HGUI_rect(xpos+3, ypos, xpos+4, ypos+3);
				HGUI_cSet(WHITE, FOREGROUND, BRIGHT);
				HGUI_text(xpos+3, ypos, "B", false, ALIGN_LEFT);
				HGUI_text(xpos+3, ypos+1, "L", false, ALIGN_LEFT);
				HGUI_text(xpos+3, ypos+2, "U", false, ALIGN_LEFT);
				break;
			case HF_CHO:
				HGUI_cSet(RED, BACKGROUND, BRIGHT);
				HGUI_rect(xpos+3, ypos, xpos+4, ypos+3);
				HGUI_cSet(WHITE, FOREGROUND, BRIGHT);
				HGUI_text(xpos+3, ypos, "C", false, ALIGN_LEFT);
				HGUI_text(xpos+3, ypos+1, "H", false, ALIGN_LEFT);
				HGUI_text(xpos+3, ypos+2, "O", false, ALIGN_LEFT);
				break;
			case HF_ETC:
				HGUI_cSet(RED, BACKGROUND, BRIGHT);
				HGUI_rect(xpos+3, ypos, xpos+4, ypos+3);
				break;
		}
	}
	HGUI_cReset();

	// Draw Card Property
	// Month
	
	char animal_string[5] = {0};
	switch(card->type)
	{
		case H_ANIM:
			
			if(HCard_isBird(card))
			{
				sprintf(animal_string, "BIRD");
			}
			else
			{
				sprintf(animal_string, "ANIM");
			}

			HGUI_cSet(YELLOW, FOREGROUND, BRIGHT);
			HGUI_text(xpos, ypos, animal_string, false, ALIGN_LEFT);
			HGUI_cReset();
			break;
	}
	HGUI_cReset();
}

void Renderer_cardsmall(int xpos, int ypos, HCard const *card)
{
	HGUI_cSet(RED, BACKGROUND, DARK);
	HGUI_cSet(WHITE, FOREGROUND, BRIGHT);
	HGUI_rect(xpos, ypos, xpos+2, ypos+2);

	switch(card->type)
	{
		case H_NORM:
			if(card->isDouble)
			{
				HGUI_text(xpos+1, ypos, "D", false, ALIGN_LEFT);
			}
			break;
		case H_ANIM:
			if(HCard_isBird(card))
			{
				HGUI_text(xpos, ypos, "B", false, ALIGN_LEFT);
			}
			else
			{
				HGUI_text(xpos, ypos, "A", false, ALIGN_LEFT);
			}
			break;
		case H_LINE:
			if(card->five_type == HF_RED)
			{
				HGUI_cSet(MAGENTA, FOREGROUND, BRIGHT);
			}
			else if(card->five_type == HF_BLU)
			{
				HGUI_cSet(BLUE, FOREGROUND, BRIGHT);
			}
			else if(card->five_type == HF_CHO)
			{
				HGUI_cSet(RED, FOREGROUND, BRIGHT);
			}
			else
			{
				HGUI_cSet(BLACK, FOREGROUND, DARK);
			}
			HGUI_text(xpos+1, ypos, "=", false, ALIGN_LEFT);
			HGUI_cSet(WHITE, FOREGROUND, BRIGHT);
			break;
		case H_GWAN:
			HGUI_text(xpos, ypos, "*", false, ALIGN_LEFT);
			break;
	}

	char month_string[3];
	sprintf(month_string, "%d", card->month);
	if(HCard_isBird(card))
	{
		HGUI_cSet(YELLOW, FOREGROUND, BRIGHT);
	}
	HGUI_text(xpos+1, ypos+1, month_string, false, ALIGN_RIGHT);
	HGUI_cReset();
}

void Renderer_deck(int xpos, int ypos, HDeck *deck, int marker, bool stack)
{
	if(deck != NULL)
	{
		int x = -1;
		int y = 0;
		HCard const *this_card = NULL;
		HCard const *prev_card = NULL;
		for(int c = 0; c < deck->size; ++c)
		{
			// Cards Drawing
			this_card = HDeck_get(deck, c)->data;
			if(c > 0 && stack)
			{
				if(this_card->month == prev_card->month)
				{
					++y;
				}
				else
				{
					++x;
					y = 0;
				}
			}
			else
			{
				++x;
			}
			Renderer_card(xpos + (CARD_WIDTH+1)*x, ypos - y, this_card);
			prev_card = this_card;

			// Marker Drawing
			HGUI_cSet(YELLOW, FOREGROUND, BRIGHT);
			if(marker >= 0)
			{
				if(marker == c)
				{
					HGUI_text(xpos + (CARD_WIDTH+1)*x + 0, ypos + CARD_HEIGHT + 1, "^", false, ALIGN_LEFT);
					HGUI_text(xpos + (CARD_WIDTH+1)*x + 2, ypos + CARD_HEIGHT + 1, "^", false, ALIGN_LEFT);
					HGUI_text(xpos + (CARD_WIDTH+1)*x + 4, ypos + CARD_HEIGHT + 1, "^", false, ALIGN_LEFT);
				}
				else
				{
					HGUI_text(xpos + (CARD_WIDTH+1)*x + 0, ypos + CARD_HEIGHT + 1, " ", false, ALIGN_LEFT);
					HGUI_text(xpos + (CARD_WIDTH+1)*x + 2, ypos + CARD_HEIGHT + 1, " ", false, ALIGN_LEFT);
					HGUI_text(xpos + (CARD_WIDTH+1)*x + 4, ypos + CARD_HEIGHT + 1, " ", false, ALIGN_LEFT);
				}
			}
			HGUI_cReset();
		}
	}
	else
	{
 #ifdef DEBUG
		printError("Renderer", "Warning", "Renderer_deck(int, int, HDeck *, int bool)", "NULL HDeck Exception");
 #endif
	}
}

void Renderer_decksmall(int xpos, int ypos, HDeck *deck)
{
	if(deck != NULL)
	{
		int x = -1;
		int y = 0;
		HCard const *this_card = NULL;
		HCard const *prev_card = NULL;
		for(int c = 0; c < deck->size; ++c)
		{
			// Cards Drawing
			this_card = HDeck_get(deck, c)->data;
			if(c > 0)
			{
				if(this_card->month == prev_card->month)
				{
					++y;
				}
				else
				{
					++x;
					y = 0;
				}
			}
			else
			{
				++x;
			}
			Renderer_cardsmall(xpos + 3*x, ypos - y, this_card);
			prev_card = this_card;

			HGUI_cReset();
		}
	}
	else
	{
 #ifdef DEBUG
		printError("Renderer", "Warning", "Renderer_decksmall(int, int, HDeck *, int bool)", "NULL HDeck Exception");
 #endif
	}	
}

void Renderer_game(HGame *me, int marker)
{
	if(me == NULL)
	{
 #ifdef DEBUG
		printError("HGame", "Error", "draw(HGame *)", "Cannot draw this game : NULL");
 #endif
		return ;
	}
	HGUI_curSet(1, 1);

	// Draw the Window
	// GENERAL WINDOW
	HGUI_window(1, 1, 2*CELL_WIDTH-1, CELL_HEIGHT);

	// Grid Pattern
	for(int y=0; y<9; ++y)
	{
		for(int x=0; x<2*CELL_WIDTH-3; ++x)
		{
			HGUI_cSet(BLACK, BACKGROUND, DARK);
			if((x+y)%2 == 0)
			{
				HGUI_cSet(BLACK, FOREGROUND, BRIGHT);
			}
			else
			{
				HGUI_cSet(BLACK, FOREGROUND, DARK);
			}
			
			HGUI_text(x+2, 2 + y, ".", false, ALIGN_LEFT);
		}
	}
	HGUI_cReset();

	// Order List
	HGUI_text(3, 2, "ACTION : ", false, ALIGN_LEFT);

	// Draw Game Cards
	HGUI_text(3, 3, "GAME CARD AREA", false, ALIGN_LEFT);

	// Gather the Card from different slot
	int x_cnt = 0;
	int y_cnt = 0;
	HCard const *crnt_card = NULL;
	HCard const *prev_card = NULL;

	Renderer_deck(3, 5, me->display_cards, -1, true);

	// Draw Player's Data
	int pbias_x = 3;
	int pbias_y = 12;
	for(int p=0; p<3; ++p)
	{
		/*
			GENERAL SECTION
		*/
		HGUI_window(1         , 1+(CELL_HEIGHT-1)*(p+1), CELL_WIDTH, CELL_HEIGHT);
		HGUI_window(CELL_WIDTH, 1+(CELL_HEIGHT-1)*(p+1), CELL_WIDTH, CELL_HEIGHT);

		// Grid Pattern
		for(int y=0; y<9; ++y)
		{
			for(int x=0; x<43; ++x)
			{
				HGUI_cSet(BLACK, BACKGROUND, DARK);
				if((x+y)%2 == 0)
				{
					HGUI_cSet(BLACK, FOREGROUND, BRIGHT);
				}
				else
				{
					HGUI_cSet(BLACK, FOREGROUND, DARK);
				}
				
				HGUI_text(x+2, 2 + (CELL_HEIGHT-1)*(p+1) + y, ".", false, ALIGN_LEFT);
				HGUI_text(x+1+CELL_WIDTH, 2 + (CELL_HEIGHT-1)*(p+1) + y, ".", false, ALIGN_LEFT);
			}
		}
		HGUI_cReset();

		// Name
		HGUI_curSet(pbias_x, pbias_y+(CELL_HEIGHT-1)*p);
		if(me->current_player_num == p)
		{
			// When Selected Turns
			HGUI_cSet(RED  , BACKGROUND, DARK);
			HGUI_cSet(BLACK, FOREGROUND, DARK);
		}
		printf("PLAYER %d : %s", p+1, (me->player)[p]->name);
		
		// Score
		char score_string[32];
		sprintf(score_string, "Score : %3d", (me->player)[p]->score);
		HGUI_text(pbias_x + 24, pbias_y + 10*p, score_string, false, ALIGN_LEFT);
		HGUI_cReset();

		/*
			PLAYER SECTION
		*/
		HDeck_sort((me->player)[p]->myDeck);
		if(me->current_player_num == p)
		{
			Renderer_deck(pbias_x, pbias_y + 2 + (CARD_HEIGHT+5)*p, (me->player)[p]->myDeck, marker, false);
		}
		else
		{
			Renderer_deck(pbias_x, pbias_y + 2 + (CARD_HEIGHT+5)*p, (me->player)[p]->myDeck, -1, false);
		}

		HGUI_cSet(RED, FOREGROUND, BRIGHT);
		HGUI_text(CELL_WIDTH + 2, pbias_y + 2 + (CARD_HEIGHT+5)*p, "PPI :", false, ALIGN_LEFT);
		HGUI_text(CELL_WIDTH + 2, pbias_y + 7 + (CARD_HEIGHT+5)*p, "ANIM:", false, ALIGN_LEFT);
		HGUI_text(CELL_WIDTH + 2, pbias_y + 6 + (CARD_HEIGHT+5)*p, "LINE:", false, ALIGN_LEFT);
		HGUI_text(CELL_WIDTH + 24, pbias_y + 6 + (CARD_HEIGHT+5)*p, "GWAN:", false, ALIGN_LEFT);
		HDeck_sort((me->player)[p]->normDeck);
		HDeck_sort((me->player)[p]->animDeck);
		HDeck_lsort((me->player)[p]->lineDeck);
		HDeck_sort((me->player)[p]->gwanDeck);
		Renderer_decksmall(CELL_WIDTH + 7, pbias_y + 2 + (CARD_HEIGHT+5)*p, (me->player)[p]->normDeck);
		Renderer_decksmall(CELL_WIDTH + 7, pbias_y + 6 + (CARD_HEIGHT+5)*p, (me->player)[p]->animDeck);
		Renderer_decksmall(CELL_WIDTH + 7, pbias_y + 5 + (CARD_HEIGHT+5)*p, (me->player)[p]->lineDeck);
		Renderer_decksmall(CELL_WIDTH + 29, pbias_y + 6 + (CARD_HEIGHT+5)*p, (me->player)[p]->gwanDeck);

		// Player State Print
		HGUI_text(SCR_WIDTH + 5, pbias_y + 2 + (CARD_HEIGHT+5)*p, (me->player)[p]->last_state, true, ALIGN_CENTER);
	}
}

/*
	DIALOGUE
*/
int Renderer_eatw(HDeck *deck)
{
	char LABEL[] = "What do you want to eat?[1/2]";
	int len = strlen(LABEL);

	HGUI_cSet(RED, BACKGROUND, DARK);
	HGUI_cSet(RED, FOREGROUND, BRIGHT);
	HGUI_window(SCR_WIDTH/2 - len/2 - 1, SCR_HEIGHT/2 - CARD_HEIGHT/2 - 2, len+4, CARD_HEIGHT + 5);

	HGUI_cSet(WHITE, FOREGROUND, BRIGHT);
	HGUI_text(SCR_WIDTH/2+1, SCR_HEIGHT/2 - CARD_HEIGHT/2 - 1, LABEL, false, ALIGN_CENTER);

	Renderer_deck(SCR_WIDTH/2 - CARD_WIDTH + 1, SCR_HEIGHT/2 - 1, deck, -1, false);
	
	while(true)
	{
		switch(HGUI_getch())
		{
			case '1':
				return 0;
				break;
			case '2':
				return 1;
				break;
		}
	}
}

void Renderer_apChange(HPlayer *player)
{
	// Search for that Card
	bool isExist = false;
	int  cPos    = -1;
	HCard const *temp = NULL;
	for(int c=0; c<player->normDeck->size; ++c)
	{
		temp = HDeck_get(player->normDeck, c)->data;
		if(temp->month == 9 && temp->type == H_ANIM)
		{
			isExist = true;
			cPos    = c;
			break;
		}
	}

	if(isExist && !(player->hasChangeAP))
	{
		char LABEL[] = "Change PPI to ANIM (Only 1 Chance) [y/n]";
	
		int len = strlen(LABEL);
	
		HGUI_cSet(RED, BACKGROUND, DARK);
		HGUI_cSet(RED, FOREGROUND, BRIGHT);
		HGUI_window(SCR_WIDTH/2 - len/2 - 1, SCR_HEIGHT/2 - CARD_HEIGHT/2 - 2, len+4, CARD_HEIGHT + 5);
	
		HGUI_cSet(WHITE, FOREGROUND, BRIGHT);
		HGUI_text(SCR_WIDTH/2+1, SCR_HEIGHT/2 - CARD_HEIGHT/2 - 1, LABEL, false, ALIGN_CENTER);
	
		Renderer_card(SCR_WIDTH/2 - CARD_WIDTH/2 + 1, SCR_HEIGHT/2 - 1, temp);
		
		while(true)
		{
			switch(HGUI_getch())
			{
				case 'y':
					HDeck_drawFrom(player->animDeck, player->normDeck, cPos);
					player->hasChangeAP = true;
					return ;
				case 'n':
					return ;
			}
		}
	}
}