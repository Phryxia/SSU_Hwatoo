#include <stdio.h>
#include "HGame.h"
#include "HGUI.h"
#include "Debug_Center.h"

int main(void)
{
	logReset();

	HGUI_erase();

	HGUI_rect(4, 4, 8, 8);
	HGUI_rect(10, 4, 14, 8);
	HGUI_rect(16, 4, 20, 8);
	
	return 0;
}
