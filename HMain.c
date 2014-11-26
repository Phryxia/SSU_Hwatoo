#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "HFlow.h"

int main(void)
{
	logReset();
	srand(time(NULL));
	Renderer_intro();

	/*
		RESOURCE INITIALIZE
	*/
	printError("Main", "Note", "main(void)", "Try to load CARD_SET...");
	HCard *CARD_SET = halloc();
	printError("Main", "Note", "main(void)", "Try to load HGame...");
	HGame *GAME = new_HGame(CARD_SET);

	if(GAME == NULL)
	{
 #ifdef DEBUG
		printError("HMain", "Error", "main(void)", "Game Load Fail");
 #endif
	}

	char const *MENU_LIST[3] = {"1. New Game", "2. Load Game", "3. Exit"};

	/*
		MENU
	*/
    int mode;
	bool gameEnd = false;
    while(!gameEnd)
    {
    	// Menu Displyaing
    	HGUI_erase();
    	HGUI_cSet(RED, FOREGROUND, DARK);
    	HGUI_window(1, 1, 80, 20);
    	HGUI_text(40, 6, "사기치면  손모가지", true, ALIGN_CENTER);
    	mode = HGUI_menu(40, 12, MENU_LIST, 3);

    	// Enter the Section
    	HGUI_erase();
        switch(mode)
        {
            case 0: 
                //game start
                HFlow_doGame(GAME, CARD_SET, false);
                break;
            case 1:
                //load game
            	if(HFileIO_loadGame(GAME, CARD_SET))
            	{
            		// Load Success
            		HFlow_doGame(GAME, CARD_SET, true);
            	}
            	else
            	{
            		// Fail
            	}
                break;
            case 2:
				gameEnd = true;
                break;
            default:
                break;//잘못된 입력 
        }
    }
	
	/*
		De-Allocation
	*/
	delete_HGame(GAME);
	delete_HCard(CARD_SET);
    return 0;
}