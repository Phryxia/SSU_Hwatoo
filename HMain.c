#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "HCard.h"
#include "HDeck.h"
#include "HPlayer.h"
#include "HGame.h"
#include "HGUI.h"
#include "Renderer.h"
#include "Debug_Center.h"

void doGame(HGame *game, HCard const *CARD_SET);
bool eat(HGame *game, int card_pointer);
void stealcard(HGame *game);
bool stop(HGame *game, int winner, bool hasGwan);

bool gameEnd = false;
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
            	HGame_reset(GAME, CARD_SET);
                doGame(GAME, CARD_SET);
                break;
            case 1:
                //load game
                printf("load game\n");
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

//int card_pointer = 0;
void doGame(HGame *game, HCard const *CARD_SET)//전체게임함수
{
	// Initialize Player Turn
	HGame_initTurn(game);
	int card_pointer = 0;
	Renderer_game(game, card_pointer);
	int loop = 0;

	bool continue_game = true;
	while(continue_game)
	{
		for(int p=0; p<3 && continue_game; ++p)
		{
			//인터페이스 : e(exit), b(잔고), h(키설명),save(저장).load
			game->current_player_num = p;
			HPlayer *player = game->player[p];
			card_pointer = 0;
			Renderer_game(game, card_pointer);	
			
			bool isSelecting = true;
			while(isSelecting)
			{
				// Get Key Input
				Renderer_game(game, card_pointer);
				int  temp = HGUI_getch();
				char temp_string[32];
				switch(temp)
				{
					default:
						// Number Input
						if(1 <= temp-'0' && temp-'0' <= 7 && temp-'0' <= player->myDeck->size)
						{
							card_pointer = temp-'0'-1;
						}
						break;

					case 'a':
						// Move Selector Left
						if(card_pointer > 0)
						{
							--card_pointer;
						}
						break;

					case 'd':
						// Move Selector Right
						if(card_pointer < player->myDeck->size - 1)
						{
							++card_pointer;
						}
						break;

					case '9':
						// Change 9-Five as PPI <-> ANIM
						Renderer_apChange(player);
						break;

					case '\n':
						// Enter
						isSelecting = false;
						break;

					case 'b':
						// See the Player's Balance
						//sprintf(temp_string, "You have %d won...", player->money);
						//Renderer_notice(temp_string, CARD_HEIGHT);
						Renderer_showBalance(game);
						break;

					case 'h':
						// Help
						Renderer_help();
						break;

					case 'e':
						// Exit
						continue_game = !Renderer_exit();
						if(!continue_game)
						{
							isSelecting = false;
						}
						break;
				}
			}

			if(!continue_game)
			{
				break;
			}
			
			//eat함수
			int  shake_num = HGame_willShake(game);
			if(shake_num == 4)
			{
				// Chong-Tong
				Renderer_notice("What the... You are president! 0~0 You win!", CARD_HEIGHT);
				player->score += 10;
				stop(game, p, false);
				Renderer_statistics(game);
				HGame_reset(game, CARD_SET);
				break;
			}
			if(shake_num)
			{
				Renderer_notice("Shake it~ Shake it~", CARD_HEIGHT);
			}
			bool hasGwan   = eat(game, card_pointer);
	
			//점수산출score함수
			HGame_calcScore(game);
	
			//났나? - 났을 때 고 혹은 스톱
			if(player->score >= 3)
			{
				/*
					Several Case

					i)  Last Loop
					ii) Other Loop
				*/
				if(player->myDeck->size == 0)
				{
					/*
						Last Loop. There are Several Case

						i)  Can Go   -> 100% WIN
						ii) Can't Go -> 100% LOSE
					*/
					if(player->score > player->score_lastgo || player->how_many_go == 0)
					{
						/*
							Can Go ( AUTO STOP )
						*/
						game->was_nagari = false;
						Renderer_notice("Good Job! You Win!", CARD_HEIGHT);
						bool world_end = stop(game, p, hasGwan);

						// Show Statistics
						Renderer_statistics(game);

						if(world_end)
						{
							HGUI_cReset();
							return ;
						}
						else
						{
							// Reset
							HGame_reset(game, CARD_SET);
							break;
						}
					}
				}
				else
				{
					/*
						Not Last Loop. If you have enough quality, you can go.
					*/
					if(player->score > player->score_lastgo || player->how_many_go == 0)
					{
						if(Renderer_willGo())
						{
							// Go
							player->how_many_go += 1;
							player->score_lastgo = player->score;
						}
						else
						{
							// Stop (or Auto Stop)
							game->was_nagari = false;
							Renderer_notice("Good Job! You Win!", CARD_HEIGHT);
							bool world_end = stop(game, p, hasGwan);
	
							// Show Statistics
							Renderer_statistics(game);
	
							if(world_end)
							{
								HGUI_cReset();
								return ;
							}
							else
							{
								// Reset
								HGame_reset(game, CARD_SET);
								break;
							}
						}
					}
				}
			}
			else
			{
				/*
					In this case, most of turn is normal.
					But when last loop & last turn, Nagari will occured.
				*/
				if(player->myDeck->size == 0)
				{
					if(p == 2)
					{
						// Nagari '~'
						game->was_nagari = true;
						Renderer_notice("Congratulation! This phase is Nagari! Next phase will make x2 money.", CARD_HEIGHT);
						
						// Show Statistics
						Renderer_statistics(game);

						// Reset Game
						HGame_reset(game, CARD_SET);
						break;
					}
				}
			}
		}
	}

	HGUI_cReset();
}

/*
	Eating Card : Return value is whether you eat the Gwang or not.
*/
bool eat(HGame *game, int card_pointer) //자기턴진행함수
{
	// Selector
    //먹을거 있니(1~7)-뭐먹을래(1~2)
	HCard const *myCard = HDeck_get(HGame_nowPlayer(game)->myDeck, card_pointer)->data;
	HCard const *topCard = game->unknown_cards->first->prev->data;
	HDeck_remove(HGame_nowPlayer(game)->myDeck, card_pointer);
	HDeck_pop(game->unknown_cards);

	HDeck *whatYouEat = new_HDeck();

    //까기
    bool hasGwan = false;
	
	if(myCard->month == topCard->month)
	{
		// New card from the Main Deck is same with myCard
		HDeck *target = (game->visible_cards)[myCard->month-1];
		
		switch(target->size)
		{
			case 0:
				// There is no matching card on the floor
				// ZOK
				HPlayer_eat(HGame_nowPlayer(game), myCard);
				HPlayer_eat(HGame_nowPlayer(game), topCard);
				HDeck_push(whatYouEat, myCard);
				HDeck_push(whatYouEat, topCard);
				if(myCard->type == H_GWAN || topCard->type == H_GWAN)
				{
					hasGwan = true;
				}

				Renderer_notice("쪽 !", CARD_HEIGHT);
				
				stealcard(game);
				break;
			case 1:
				// POO
				HDeck_push(target, myCard);
				HDeck_push(target, topCard);
				
				Renderer_notice("쌌다 !", CARD_HEIGHT);
				break;
			case 2:
				// 4장 먹기
				HPlayer_eat(HGame_nowPlayer(game), myCard);
				HPlayer_eat(HGame_nowPlayer(game), topCard);
				HPlayer_eat(HGame_nowPlayer(game), HDeck_get(target, 0)->data);
				HPlayer_eat(HGame_nowPlayer(game), HDeck_get(target, 1)->data);
				HDeck_push(whatYouEat, myCard);
				HDeck_push(whatYouEat, topCard);
				HDeck_push(whatYouEat, HDeck_get(target, 0)->data);
				HDeck_push(whatYouEat, HDeck_get(target, 1)->data);

				// Gwang Processing
				if(myCard->type == H_GWAN || topCard->type == H_GWAN ||
					HDeck_get(target, 0)->data->type == H_GWAN ||
					HDeck_get(target, 1)->data->type == H_GWAN)
				{
					hasGwan = true;
				}
				HDeck_clear(target);
				
				Renderer_notice("따닥 !", CARD_HEIGHT);

				stealcard(game);
				break;
		}
	}
	else
	{
		for(int t=0; t<2; ++t)
		{
			// If it's not, you should take two possibilities.
			HCard const *nowCard;
			HDeck *target;

			// Set Target
			// 1. Your Card, 2. Top Card from Deck
			if(t == 0)
			{
				nowCard = myCard;
				target = (game->visible_cards)[myCard->month-1]; // Bcz of Horrible Length....
			}
			else
			{
				nowCard = topCard;
				target = (game->visible_cards)[topCard->month-1];
			}

			int sel_num;
			switch(target->size)
			{
				case 0:
					// You Throw the Card
					HDeck_push(target, nowCard);
					break;
				case 1:
					// Match : You Can Eat This!
					HPlayer_eat(HGame_nowPlayer(game), nowCard);
					HPlayer_eat(HGame_nowPlayer(game), HDeck_get(target, 0)->data);
					HDeck_push(whatYouEat, nowCard);
					HDeck_push(whatYouEat, HDeck_get(target, 0)->data);
					
					// Gwang Processing
					if(nowCard->type == H_GWAN || HDeck_get(target, 0)->data->type == H_GWAN)
					{
						hasGwan = true;
					}

					HDeck_clear(target);
					
					break;
				case 2:
					// You have to select the card
					sel_num = Renderer_eatw(target);
					HPlayer_eat(HGame_nowPlayer(game), nowCard);
					HPlayer_eat(HGame_nowPlayer(game), HDeck_get(target, sel_num)->data);
					HDeck_push(whatYouEat, nowCard);
					HDeck_push(whatYouEat, HDeck_get(target, sel_num)->data);
					
					// Gwang Processing
					if(nowCard->type == H_GWAN || HDeck_get(target, sel_num)->data->type == H_GWAN)
					{
						hasGwan = true;
					}

					HDeck_remove(target, sel_num);
					break;
				case 3:
					// Eat the Poo!!!
					for(int i=0; i<3; ++i)
					{
						// Eat Every Cards in the Deck
						HPlayer_eat(HGame_nowPlayer(game), HDeck_get(target, i)->data);
						HDeck_push(whatYouEat, HDeck_get(target, i)->data);
					}
					HPlayer_eat(HGame_nowPlayer(game), nowCard);
					HDeck_push(whatYouEat, nowCard);
					
					// Gwang Processing
					if(myCard->type == H_GWAN || topCard->type == H_GWAN ||
						HDeck_get(target, 0)->data->type == H_GWAN ||
						HDeck_get(target, 1)->data->type == H_GWAN ||
						HDeck_get(target, 2)->data->type == H_GWAN)
					{
						hasGwan = true;
					}

					HDeck_clear(target);
					
					Renderer_notice("올 ㅋ", CARD_HEIGHT);
					
					stealcard(game);
					break;
			}
		}
	}

	// Pan-SSuel-I
	if(game->display_cards->size == 0)
	{
		stealcard(game);
		Renderer_notice("Incredible!!! You clean the floor!!!", CARD_HEIGHT);
	}

	// Renderer
	if(whatYouEat->size > 0)
	{
		HDeck_sort(whatYouEat);
		Renderer_noticeCards(whatYouEat);
	}
	delete_HDeck(whatYouEat);

	return hasGwan;
}

/*
	ABOUT STEAL CARD

	When special events occured, you can grab someone's PPI.
	This will search for "Single PPI" first.
	If there is no Single PPI, it will try to grab "Double PPI".
*/
void stealcard(HGame *game)//상대피 뺏어오기 함수
{
	if(game->current_player_num < 0 || game->current_player_num >= 3)
	{
 #ifdef DEBUG
		printError("HMain", "Error", "stealcard(HGame *, int)", "Wrong Player Number");
 #endif
	}
	else
	{
		HPlayer *me  = HGame_nowPlayer(game);
		HPlayer *you = NULL;
		for(int i=0; i<3; ++i)
		{
			// Player Shadow Set
			you = (game->player)[i];
			if(you != me)
			{
				//상대피에 피가 있나?
				if(you->normDeck->size > 0)
				{
					//상대피에 쌍피만 있는가?
					// Search for Double PPI
					int target = 0;
					for(int j=0; j<you->normDeck->size; ++j)
					{
						// Check All Norm Deck Card
						if(!HDeck_get(you->normDeck, j)->data->isDouble)
						{
							// This part will search Non-Double Card.
							// If there is no Non-Double, just pick one.
							target = j;
							break;
						}
					}
					
					//쌍피만 있다면 쌍피를 가져온다
					//쌍피만 있는게 아니라면 그냥 피를 가져온다
					HDeck_drawFrom(me->normDeck, you->normDeck, target);
				}
				HDeck_sort(me->normDeck);
				HDeck_sort(you->normDeck);
			}
		}
	}
}

/*
	ABOUT STOP

	stop() will transform "Game Point" to "Money".
	It'll also
*/
bool stop(HGame *game, int winner, bool hasGwan)
{
	// Original Logic Idea by Jeong Min Gi
	HPlayer *win_player = game->player[winner];

	int money_base = WON_PER_POINT*win_player->score; // Default Prize
	bool gameover = false;
    
    //player1이 났을 시 쓰리고, 포고, 흔들기를 고려해 점수를 곱하여 보낸다
    // Processing Go
   	money_base += WON_PER_POINT*win_player->how_many_go;
    if(win_player->how_many_go >= 3)
    {
    	money_base *= (2 << (win_player->how_many_go-3));
    }

    // Processing Nagari;
    if(game->was_nagari)
    {
    	money_base *= 2;
    }

    // Processing Meong-Bak
    if(win_player->animDeck->size >= 7)
    {
    	money_base *= 2;
    }

    // Show me your Money
    for(int p=0; p<3; ++p)
    {
    	// Processing Private Transfer Rate
    	HPlayer *player = game->player[p];
    	int xRate = 1;
    	if(p != winner)
    	{
    		// Ppi-Bak
    		if(0 < player->normDeck->size && player->normDeck->size <= 5)
    		{
    			xRate *= 2;
    		}

    		// Gwang-Bak
    		if(hasGwan && player->gwanDeck->size == 0)
    		{
    			xRate *= 2;
    		}

    		/*
    			ABOUT MONEY TRANSFER

				i)  They have enough money       -> Show me the money
				ii) They don't have enough money -> Kick Out
    		*/
    		if(player->money >= money_base)
    		{
    			// Can Afford to Pay
    			win_player->money += money_base;
    			player->money -= money_base;
    		}
    		else
    		{
    			// Cannot Afford to Pay
    			win_player->money += player->money; // Their all possesion
    			player->money = 0;
    		}

    		if(player->money == 0)
    		{
    			// Kick Out
    			player->hasNoMoney = true;

    			Renderer_notice("You got financial bankrupt. So you called the police. You guys are arrested.", CARD_HEIGHT);
    			gameover = true;
    		}
    	}
    }

    HGame_setTurn(game, winner);

    return gameover; // Whether Bankrupt or not
}