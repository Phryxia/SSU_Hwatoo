#include "HGUI.h"

void setColor(int color, int mode, int brightness)
{
	if(0 <= color && color <= 9)
	{
		if(mode)
		{
			// BACKGROUND
			switch(brightness)
			{
				case BRIGHT:
					printf(BBCOLOR(color));
					break;
				case DARK:
					printf(BDCOLOR(color));
					break;
			}
		}
		else
		{
			// FOREGROUND
			switch(brightness)
			{
				case BRIGHT:
					printf(FBCOLOR(color));
					break;
				case DARK;
					printf(FDCOLOR(color));
					break;
			}
		}
	}
}

void drawCard(int xpos, int ypos, char c)
{
	setCursor(xpos, ypos);
	setColor(RED, FOREGROUND, BRIGHT);
	printf("###");
	setCursor(xpos, (ypos+1));
	printf("#%c#", c);
	setCursor(xpos, (ypos+1));
	printf("###");
	setCursor(xpos, (ypos+1));
	printf("###");
}