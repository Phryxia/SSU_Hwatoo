#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <termios.h>
#include "HGUI.h"
#include "HCard.h"
#include "Debug_Center.h"

int HGUI_getch(void)
{
	// Thanks for Iksuplorer lol.
	int ch;
	struct termios buf;
	struct termios save;
	tcgetattr(0, &save);
	buf = save;
	buf.c_lflag &= ~(ICANON | ECHO);
	buf.c_cc[VMIN]  = 1;
	buf.c_cc[VTIME] = 0;
	tcsetattr(0, TCSAFLUSH, &buf);
	ch = getchar();
	tcsetattr(0, TCSAFLUSH, &save);
	return ch;
}

void HGUI_cSet(int color, int mode, int brightness)
{
	if(mode)
	{
		if(brightness)
		{
			printf("\e[10");
		}
		else
		{
			printf("\e[4");
		}
	}
	else
	{
		if(brightness)
		{
			printf("\e[9");
		}
		else
		{
			printf("\e[3");
		}
	}
	if(0 <= color && color <= 7)
	{
		printf("%d", color);
	}
	else
	{
		printf("9");
	}
	printf("m");
}

void HGUI_cReset(void)
{
	printf("\e[39m\e[49m");
}

void HGUI_erase(void)
{
	printf("\e[2J\e[0;0H");
}

void HGUI_curSet(int xpos, int ypos)
{
	printf("\e[%d;%dH", ypos, xpos);
}

void _HGUI_check(int *x, int *y)
{
	// This is inter-function. You can't use this outside.
	if(*x > *y)
	{
		int temp = *x;
		*x = *y;
		*y = temp;
	}
}

void HGUI_rect(int x1, int y1, int x2, int y2)
{
	_HGUI_check(&x1, &x2);
	_HGUI_check(&y1, &y2);
	for(int y=y1; y<y2; ++y)
	{
		HGUI_curSet(x1, y);
		for(int x=x1; x<x2; ++x)
		{
			printf(" ");
		}
	}
}

void HGUI_window(int x1, int y1, int x2, int y2)
{
	_HGUI_check(&x1, &x2);
	_HGUI_check(&y1, &y2);
	for(int y=y1; y<y2; ++y)
	{
		HGUI_curSet(x1, y);
		if(y == y1 || y == y2-1)
		{
			HGUI_cSet(RED, FOREGROUND, DARK);
			for(int x=x1; x<x2; ++x)
			{
				printf("*");
			}
			HGUI_cReset();
		}
		else
		{
			HGUI_cSet(RED, FOREGROUND, DARK);
			printf("*");
			for(int x=x1+1; x<x2-1; ++x)
			{
				printf(" ");
			}
			printf("*");
			HGUI_cReset();
		}
	}
}

void HGUI_text(int xpos, int ypos, char const *text, bool wide, int mode)
{
	switch(mode)
	{
		case ALIGN_LEFT:
			HGUI_curSet(xpos, ypos);
			break;
		case ALIGN_CENTER:
			if(wide)
			{
				HGUI_curSet(xpos-strlen(text)/3, ypos);
			}
			else
			{
				HGUI_curSet(xpos-strlen(text)/2, ypos);
			}
			break;
		case ALIGN_RIGHT:
			HGUI_curSet(xpos-strlen(text), ypos);
			break;
	}
	printf("%s", text);
}

int  HGUI_menu(int xpos, int ypos, char const **strings, int m_length)
{
	// Pointer Memorizer
	static int pointer = 0;
	bool flag = true;
	while(flag)
	{
		/*
			Draw the Menu
		*/
		for(int i=0; i<m_length; ++i)
		{
			if(pointer == i)
	    	{
	    		HGUI_cSet(RED  , BACKGROUND, DARK);
	    		HGUI_cSet(WHITE, FOREGROUND, BRIGHT);
	    	}
	    	else
	    	{
	    		HGUI_cSet(RESET, BACKGROUND, DARK);
	    		HGUI_cSet(RED  , FOREGROUND, BRIGHT);
	    	}
	    	HGUI_text(xpos, ypos + 2*(i-m_length/2), strings[i], false, ALIGN_CENTER);
		}
		HGUI_cReset();
	
		/*
			Input Key and Move Pointer or Select Returns
		*/
		int command;
		switch((command = HGUI_getch()))
		{
			default:
				if('1' <= command && command <= '9')
				{
					pointer = command-'1';
				}
				break;
			case 'w':
				if(0 < pointer)
				{
					--pointer;
				}
				break;
			case 's':
				if(pointer < m_length-1)
				{
					++pointer;
				}
				break;
			case '\n':
				flag = false;
				break;
		}
	}
	return pointer;
}

void HGUI_card(int xpos, int ypos, HCard const *card)
{
	if(card == NULL)
	{
 #ifdef DEBUG
		printError("HGUI", "Error", "HGUI_card", "NULL Card Exception");
 #endif
	}

	char month_string[3] = {0};
	sprintf(month_string, "%2d", card->month);
	switch(card->month)
	{
		case 1:
			HGUI_cSet(WHITE, BACKGROUND, BRIGHT);
			HGUI_rect(xpos, ypos, xpos+5, ypos+5);

			HGUI_cSet(BLACK, BACKGROUND, BRIGHT);
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

			HGUI_cSet(WHITE, FOREGROUND, BRIGHT);
			HGUI_text(xpos+4, ypos+4, "1", false, ALIGN_LEFT);

			if(card->type == H_GWAN)
			{
				HGUI_cSet(WHITE, FOREGROUND, BRIGHT);
				HGUI_text(xpos, ypos+4, "*", false, ALIGN_LEFT);

				HGUI_cSet(BLACK, BACKGROUND, DARK);
				HGUI_cSet(WHITE, FOREGROUND, BRIGHT);
				HGUI_text(xpos, ypos, "GWAN", false, ALIGN_LEFT);
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

			HGUI_cSet(BLACK, FOREGROUND, DARK);
			HGUI_text(xpos+4, ypos+4, "2", false, ALIGN_LEFT);
			break;
		case 3:
			HGUI_cSet(WHITE, BACKGROUND, BRIGHT);
			HGUI_rect(xpos, ypos, xpos+5, ypos+5);

			if(card->type == H_GWAN)
			{
				HGUI_cSet(BLACK, FOREGROUND, DARK);
				HGUI_text(xpos, ypos+4, "*", false, ALIGN_LEFT);

				HGUI_cSet(BLACK, BACKGROUND, DARK);
				HGUI_cSet(WHITE, FOREGROUND, BRIGHT);
				HGUI_text(xpos, ypos, "GWAN", false, ALIGN_LEFT);
			}

			HGUI_cSet(MAGENTA, BACKGROUND, BRIGHT);
			HGUI_rect(xpos, ypos+3, xpos+3, ypos+4);
			HGUI_rect(xpos+1, ypos+2, xpos+2, ypos+5);
			HGUI_rect(xpos+3, ypos+4, xpos+5, ypos+5);
			HGUI_rect(xpos+4, ypos+3, xpos+5, ypos+4);

			HGUI_cSet(MAGENTA, BACKGROUND, DARK);
			HGUI_rect(xpos+1, ypos+3, xpos+2, ypos+4);
			HGUI_rect(xpos+4, ypos+4, xpos+5, ypos+5);
			HGUI_cSet(WHITE, FOREGROUND, BRIGHT);
			HGUI_text(xpos+4, ypos+4, "3", false, ALIGN_LEFT);
			
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

			HGUI_text(xpos+4, ypos+4, "4", false, ALIGN_LEFT);
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			break;
	}
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