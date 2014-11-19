#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <termios.h>
#include "HGUI.h"
#include "HCard.h"
#include "HDeck.h"
#include "HGame.h"
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

void HGUI_window(int xpos, int ypos, int xsize, int ysize)
{
	for(int y=0; y<ysize; ++y)
	{
		HGUI_curSet(xpos, ypos+y);
		if(y == 0 || y == ysize-1)
		{
			// First Line & Last Line
			for(int x=0; x<xsize; ++x)
			{
				printf("*");
			}
		}
		else
		{
			printf("*");
			for(int x=1; x<xsize-1; ++x)
			{
				printf(" ");
			}
			printf("*");
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
			HGUI_curSet(xpos-strlen(text)+1, ypos);
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