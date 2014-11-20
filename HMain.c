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

void doGame(HGame *game);
void eat(HGame *game);
void stealcard(HGame *game);
void stop(void);

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
                doGame(GAME);
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
        getchar();
    }
	
	/*
		De-Allocation
	*/
	delete_HGame(GAME);
	delete_HCard(CARD_SET);
    return 0;
}

int card_pointer = 0;
void doGame(HGame *game)//전체게임함수
{
	// Initialize Player Turn
	HGame_initTurn(game);
	Renderer_game(game, card_pointer);

	bool continue_game = true;

	while(continue_game)
	{
		for(int p=0; p<3; ++p)
		{
			//인터페이스 : e(exit), b(잔고), h(키설명),save(저장).load
			HPlayer *player = game->player[p];
			card_pointer = 0;
			Renderer_game(game, card_pointer);	
			
			bool isSelecting = true;
			while(isSelecting)
			{
				// Get Key Input
				Renderer_game(game, card_pointer);
				int temp = HGUI_getch();
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
						break;

					case 'h':
						// Help
						break;

					case 'e':
						// Exit
						continue_game = false;
						break;
				}
			}
			
			//eat함수 
			eat(game);
	
			//점수산출score함수
			HGame_calcScore(game);
	
			//났나? - 났을 때 고 혹은 스톱
	
			//스톱시 stop함수
	
			//턴 넘기기
			HGame_moveTurn(game);
		}
	}
}

/*
	eat 함수의 리턴 값은 현재 플레이어가 패를 뺏아오는 상황인지 아닌지 결정한다
*/
void eat(HGame *game) //자기턴진행함수
{
	// Selector
    //먹을거 있니(1~7)-뭐먹을래(1~2)
	HCard const *myCard = HDeck_get(HGame_nowPlayer(game)->myDeck, card_pointer)->data;
	HCard const *topCard = game->unknown_cards->first->prev->data;
	HDeck_remove(HGame_nowPlayer(game)->myDeck, card_pointer);
	HDeck_pop(game->unknown_cards);

	HDeck *whatYouEat = new_HDeck();

    //까기
    bool hasZok = false;
    bool hasPoo = false;
    bool eatPoo = false;
    bool hasDdk = false;
	
	if(myCard->month == topCard->month)
	{
		// New card from the Main Deck is same with myCard
		HDeck *target = (game->visible_cards)[myCard->month-1];
		
		switch(target->size)
		{
			case 0:
				// There is no matching card on the game
				// ZOK
				hasZok = true;
				HPlayer_eat(HGame_nowPlayer(game), myCard);
				HPlayer_eat(HGame_nowPlayer(game), topCard);
				HDeck_push(whatYouEat, myCard);
				HDeck_push(whatYouEat, topCard);

				Renderer_notice("쪽 !", CARD_HEIGHT);
				
				stealcard(game);
				break;
			case 1:
				// POO
				hasPoo = true;
				HDeck_push(target, myCard);
				HDeck_push(target, topCard);
				
				Renderer_notice("쌌다 !", CARD_HEIGHT);
				break;
			case 2:
				// 4장 먹기
				eatPoo = true;
				HPlayer_eat(HGame_nowPlayer(game), myCard);
				HPlayer_eat(HGame_nowPlayer(game), topCard);
				HPlayer_eat(HGame_nowPlayer(game), HDeck_get(target, 0)->data);
				HPlayer_eat(HGame_nowPlayer(game), HDeck_get(target, 1)->data);
				HDeck_push(whatYouEat, myCard);
				HDeck_push(whatYouEat, topCard);
				HDeck_push(whatYouEat, HDeck_get(target, 0)->data);
				HDeck_push(whatYouEat, HDeck_get(target, 1)->data);
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
			// 1. Your Card, 2. Top Card from Deck
			HCard const *nowCard;
			HDeck *target;

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
					HDeck_clear(target);
					break;
				case 2:
					// You have to select the card
					sel_num = Renderer_eatw(target);
					HPlayer_eat(HGame_nowPlayer(game), nowCard);
					HPlayer_eat(HGame_nowPlayer(game), HDeck_get(target, sel_num)->data);
					HDeck_push(whatYouEat, nowCard);
					HDeck_push(whatYouEat, HDeck_get(target, sel_num)->data);
					HDeck_remove(target, sel_num);
					break;
				case 3:
					// Eat the Poo!!!
					eatPoo = true;
					for(int i=0; i<3; ++i)
					{
						// Eat Every Cards in the Deck
						HPlayer_eat(HGame_nowPlayer(game), HDeck_get(target, i)->data);
						HDeck_push(whatYouEat, HDeck_get(target, i)->data);
					}
					HPlayer_eat(HGame_nowPlayer(game), nowCard);
					HDeck_push(whatYouEat, nowCard);
					HDeck_clear(target);
					
					Renderer_notice("올 ㅋ", CARD_HEIGHT);
					
					stealcard(game);
					break;
			}
		}

		/*
		// 2. New Card
		target = (game->visible_cards)[topCard->month-1];
		switch(target->size)
		{
			case 0:
				// You Throw the Card
				HDeck_push(target, topCard);
				break;
			case 1:
				// You Can Eat This!
				HPlayer_eat(HGame_nowPlayer(game), HDeck_get(target, 0)->data);
				HPlayer_eat(HGame_nowPlayer(game), topCard);
				HDeck_clear(target);

				Renderer_noticeCard(topCard);
				break;
			case 2:
				// You have to select the card
				sel_num = Renderer_eatw(target);
				HPlayer_eat(HGame_nowPlayer(game), HDeck_get(target, sel_num)->data);
				HDeck_remove(target, sel_num);
				
				Renderer_noticeCard(topCard);
				break;
			case 3:
				// Eat the Poo!!!
				eatPoo = true;
				for(int i=0; i<3; ++i)
				{
					HPlayer_eat(HGame_nowPlayer(game), HDeck_get(target, i)->data);
				}
				HPlayer_eat(HGame_nowPlayer(game), topCard);
				HDeck_clear(target);
				
				Renderer_notice("개이득 !", CARD_HEIGHT);
				Renderer_noticeCard(topCard);
				
				stealcard(game);
				break;
		}
		*/
	}

	if(whatYouEat->size > 0)
	{
		HDeck_sort(whatYouEat);
		Renderer_noticeCards(whatYouEat);
	}
	delete_HDeck(whatYouEat);
}

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

void stop(void)
{
    //player1이 났을 시 쓰리고, 포고, 흔들기를 고려해 점수를 곱하여 보낸다

    //플레이어2에게 보낸다

    //플레이어 2의 광박 , 피박, 멍텅구리 여부를 파악해 곱한다.

    //돈으로 환산해 플레이어 1에게 보낸다.

    //만약 파산했을 경우, 0까지만 뺀 뒤에 빠진만큼 플레이어1에게 더하고 게임을 종료한다.

    //플레이어 3에게 플레이어1의 점수를 보낸다.

    //플레이어 3의 광박, 피박, 멍텅구리 여부를 파악해 곱한다.

    //돈으로 환산해 플레이어 1에게 보낸다.

    //만약 파산했을 경우, 0까지만 뺀 뒤에 빠진만큼 플레이어1에게 더하고 게임을 종료한다.

    //패를 섞은 후 다시 시작한다.
}