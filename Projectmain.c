#include <stdio.h>
#include <termios.h>
#include <stdbool.h>
#include <ncurses.h>
#include "HGame.h"

void game(void);
void eat(void);
void score(void);
void stealcard(void);
void stop(void);
int getch(void); // Attribution by Iksuplorer

bool gameEnd = false;
int main(void)
{
	/*
		RESOURCE INITIALIZE
	*/
	printError("Main", "Note", "main(void)", "Try to load CARD_SET...");
	HCard *CARD_SET = halloc();
	printError("Main", "Note", "main(void)", "Try to load HGame...");
	HGame *GAME = new_HGame(CARD_SET);
	
	/*
		MENU
	*/
    int mode;
	
    while(!gameEnd)
    {
    	
        printf("1.new game\n2.load game\n3.exit");
        switch((mode = getch()) - '0')
        {
            case 1: 
                //game start
                printf("new game\n");
                break;
            case 2:
                //load game
                printf("load game\n");
                break;
            case 3:
                printf("exit\n");
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

void game(HGame *game)//전체게임함수
{
	int current_player = 0;
	while(true)
	{
		//인터페이스 : e(exit), b(잔고), h(키설명),save(저장).load
		
		//eat함수 
		eat();

		//상태갱신

		//점수산출score함수
		score();

		//났나? - 났을 때 고 혹은 스톱

		//스톱시 stop함수

		//턴 넘기기
		++current_player;
	}
}

/*
	eat 함수의 리턴 값은 현재 플레이어가 패를 뺏아오는 상황인지 아닌지 결정한다
*/
void eat(HGame *game, int player_num)//자기턴진행함수
{
    //먹을거 있니(1~7)-뭐먹을래(1~2)
    HPlayer *current_player = game->player[player_num];
	int select;
	while(true)
	{
		select = getch() - '0';
		if(0 <= select && select <= 7)
		{
			break;
		}
	}
	HCard *myCard = current_player->myDeck->get(current_player->myDeck, select)->data;
	HCard *topCard = game->unknown_cards->first->prev->data;
	current_player->myDeck->remove(current_player->myDeck, select);
	game->unknown_cards->pop(game->unknown_cards);

    //까기
    bool hasZok = false;
    bool hasPoo = false;
    bool eatPoo = false;
    bool hasDdk = false;
	
	if(myCard->month == topCard->month)
	{
		switch(game->visible_cards[myCard->month-1]->size)
		{
			case 0:
				// There is no matching card on the game
				// ZOK
				hasZok = true;
				current_player->eat(current_player, myCard);
				current_player->eat(current_player, topCard);
				break;
			case 1:
				// POO
				hasPoo = true;
				game->visible_cards->push(game->visible_cards, myCard);
				game->visible_cards->push(game->visible_cards, topCard);
				break;
			case 2:
				// 4장 먹기
				eatPoo = true;
				current_player->eat(current_player, myCard);
				current_player->eat(current_player, topCard);
				current_player->eat(current_player, game->visible_cards[myCard->month-1]->first);
				current_player->eat(current_player, game->visible_cards[myCard->month-1]->first->next);
				game->visible_cards[myCard->month-1]->clear(game->visible_cards[myCard->month-1]);
				break;
		}
	}
	else
	{
	}



    //쌋냐
	
	/*
	if(~~~) // 조건 필요함
	{
		hasPoo = true;
	}
	else
	{
		hasPoo = false;
	}
	*/

    //따악-stillcard
	if(false) // Need Condition
	{
		stealcard(game, player_num);
	}
	
    //싼거먹기-stillcrad 
	if(false) // Need Condition
	{
		stealcard(game, player_num);
	}
}

void score(void)//점수산출함수
{
    //인자에 점수산출을 넣거나 점수계산

    //광점수 계산

    //십 점수 계산

    //띠 점수 계산

    //피 점수 계산

    //더한값을 리턴
}

void stealcard(HGame *game, int player_num)//상대피 뺏어오기 함수
{
	if(player_num < 0 || player_num >= 3)
	{
#ifdef DEBUG
		printError("HMain", "Error", "stealcard(HGame *, int)", "Wrong Player Number");
#endif
	}
	else
	{
		HPlayer *me  = game->player[player_num];
		HPlayer *you = NULL;
		for(int i=0; i<3; ++i)
		{
			you = game->player[i];
			if(i != player_num)
			{
				//상대피에 피가 있나?
				if(you->normDeck->size > 0)
				{
					//상대피에 쌍피만 있는가?
					// Search for Double PPI
					int  target = 0;
					for(int j=0; j<you->normDeck->size; ++j)
					{
						// Check All Norm Deck Card
						if(!you->normDeck->get(you->normDeck, j)->data->isDouble)
						{
							// This part will search Non-Double Card. If there is no Non-Double, just pick one.
							target = j;
							break;
						}
					}
					
					//쌍피만 있다면 쌍피를 가져온다
					//쌍피만 있는게 아니라면 그냥 피를 가져온다
					me->drawFrom(me, you, target);
				}
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

int getch(void)
{
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