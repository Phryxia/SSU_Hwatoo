#include <stdio.h>
#include "HGUI.h"

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

void HGUI_rect(int x1, int y1, int x2, int y2)
{
	if(x1 > x2)
	{
		int temp = x1;
		x1 = x2;
		x2 = temp;
	}
	if(y1 > y2)
	{
		int temp = y1;
		y1 = y2;
		y2 = temp;
	}
	for(int y=y1; y<y2; ++y)
	{
		HGUI_curSet(x1, y);
		HGUI_cSet(RED, BACKGROUND, DARK);
		for(int x=x1; x<x2; ++x)
		{
			printf(" ");
		}
		HGUI_cReset();
	}
}