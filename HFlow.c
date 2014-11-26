#include "HFlow.h"

void HFlow_doGame(HGame *game, HCard const *CARD_SET, bool wasLoaded)//전체게임함수
{
	// Initialize Player Turn
	if(!wasLoaded)
	{
		while(true)
		{
			/*
				Four Same-Month-Card Protection. (It'll never end)
			*/
			HGame_reset(game, CARD_SET);

			bool four_card_duplication = false;
			int same_counter = 1;
			HCard const *prev_card = HDeck_get(game->display_cards, 0)->data;
			HCard const *this_card = NULL;
			for(int c=1; c<game->display_cards->size; ++c)
			{
				this_card = HDeck_get(game->display_cards, c)->data;

				if(this_card->month == prev_card->month)
				{
					++same_counter;

					if(same_counter == 4)
					{
						four_card_duplication = true;
						break;
					}
				}
			}
			if(!four_card_duplication)
			{
				break;
			}
		}
	}
	Renderer_help();

	/*
		ABOUT CHONG-TONG
		Chong-tong should exist only one.
		If there are more than 2 chong-tong, game will be nagari.
		To examine whether chong-tong or not, this iterate every cards they have.
	*/
	// Chong-Tong Examination
	int pres_who  = -1;
	int pres_flag = HGame_isPres(game, &pres_who);
	
	if(pres_flag == 1)
	{
		// Chong-Tong
		game->player[pres_who]->score += 10;
		
		Renderer_notice("What the... You are president! 0~0 You win!", CARD_HEIGHT);
		HFlow_stop(game, pres_who, false);
		Renderer_statistics(game);
		HGame_reset(game, CARD_SET);
	}
	else if(pres_flag == -1)
	{
		// Nagari '~'
		game->was_nagari = true;

		Renderer_notice("Congratulation! This phase is Nagari! Next phase will make x2 money.", CARD_HEIGHT);
		Renderer_statistics(game);
		HGame_reset(game, CARD_SET);
	}

	int card_pointer = 0;
	Renderer_game(game, card_pointer);

	bool continue_game = true;
	while(continue_game)
	{
		// When loaded, first time is preserved turn.
		// But after that, first time is 0 Player's turn.
		int p;
		if(wasLoaded)
		{
			p = game->current_player_num;
			wasLoaded = false;
		}
		else
		{
			p = 0;
		}
		for( ; p<3 && continue_game; ++p)
		{
			
			game->current_player_num = p;
			HPlayer *player = game->player[p];
			card_pointer = 0;
			Renderer_game(game, card_pointer);

			if(HGame_willShake(game))
			{	
				Renderer_notice("Shake it~ Shake it~", CARD_HEIGHT);
			}

			//인터페이스 : e(exit), b(잔고), h(키설명),save(저장).load
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

					case 's':
						if(Renderer_save())
						{
							if(HFileIO_saveGame(game, CARD_SET))
							{
								Renderer_notice("Save Done!", CARD_HEIGHT);
							}
							else
							{
								Renderer_notice("Save Fail!", CARD_HEIGHT);
							}
						}
						break;
				}
			}

			if(!continue_game)
			{
				break;
			}
			
			//eat함수
			bool hasGwan = HFlow_eat(game, card_pointer);

			// Pan-SSuel-I
			if(game->display_cards->size == 0 && player->myDeck->size > 0)
			{
				HFlow_stealcard(game);
				HNotice_FClr(player);
			}
	
			//점수산출score함수
			HGame_calcScore(game);
	
			// End Gmae Logic
			bool loop_cut = false;
			switch(HFlow_isEnd(game))
			{
				case 2:
					// Game End with Nagari
					HNotice_Nagari();
					HGame_reset(game, CARD_SET);
					loop_cut = true;
					break;
				case 1:
					// Game End with Winner
					HNotice_Win(player);

					if(HFlow_stop(game, p, hasGwan))
					{
						// Bankrupt
						continue_game = false;
					}
					else
					{
						// Normal
						HGame_reset(game, CARD_SET);
					}
					loop_cut = true;
					break;
			}
			HGUI_cReset();

			if(loop_cut)
			{
				break;
			}
		}
	}
	HGUI_cReset();
}

/*
	ABOUT isEnd

	This will determine whether game should be end or not.
	There are three return value.

	2 : Game End with Nagari
	1 : Game End with Winner
	0 : Game Continue
*/
int HFlow_isEnd(HGame *game)
{
	//났나? - 났을 때 고 혹은 스톱
	HPlayer *player = HGame_nowPlayer(game);
	if(player->score >= 3)
	{
		/*
			Several Cas
			i)  Last Loop
			ii) Other Loop
		*/
		if(player->myDeck->size == 0)
		{
			/*
				Last Loop. There are Several Cas
				i)  Can Go   -> 100% WIN
				ii) Can't Go -> 100% LOSE
			*/
			if(player->score > player->score_lastgo || player->how_many_go == 0)
			{
				/*
					Can Go ( AUTO STOP )
				*/
				game->was_nagari = false;
				return 1;
			}
			else
			{
				return 0;
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
					return 0;
				}
				else
				{
					// Stop (or Auto Stop)
					game->was_nagari = false;
					return 1;
				}
			}
			else
			{
				return 0;
			}
		}
	}
	else
	{
		/*
			In this case, most of turn is normal.
			But when last loop & last turn, Nagari will occured.
		*/
		if(player->myDeck->size == 0 && game->current_player_num == 2)
		{
			// Nagari '~'
			game->was_nagari = true;
			return 2;
		}
		else
		{
			return 0;
		}
	}
}

/*
	Eating Card : Return value is whether you eat the Gwang or not.
*/
bool HFlow_eat(HGame *game, int card_pointer) //자기턴진행함수
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
				
				HFlow_stealcard(game);
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

				HFlow_stealcard(game);
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
					if(HCard_comp(HDeck_get(target, 0)->data, HDeck_get(target, 1)->data) == 0)
					{
						sel_num = 0;
					}
					else
					{
						sel_num = Renderer_eatw(target);
					}
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
					
					HFlow_stealcard(game);
					break;
			}
		}
	}

	// Renderer
	if(whatYouEat->size > 0)
	{
		HDeck_sort(whatYouEat);
		Renderer_noticeCards(whatYouEat);
	}
	HGame_refresh(game);

	delete_HDeck(whatYouEat);

	return hasGwan;
}

/*
	ABOUT STEAL CARD

	When special events occured, you can grab someone's PPI.
	This will search for "Single PPI" first.
	If there is no Single PPI, it will try to grab "Double PPI".
*/
void HFlow_stealcard(HGame *game)//상대피 뺏어오기 함수
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
*/
bool HFlow_stop(HGame *game, int winner, bool hasGwan)
{
	// Original Logic Idea by Jeong Min Gi
	HPlayer *win_player = game->player[winner];

	int money_base = WON_PER_POINT*win_player->score; // Default Prize
	bool gameover = false;
    
    /*
    	WINNER PART
    */
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
    	HNotice_MBak(win_player);
    }

    // Processing Player's Shakability
    for(int m=0; m<12; ++m)
    {
    	if(win_player->shaked[m])
    	{
    		// If he shaked specific month, multiple 2
    		money_base *= 2;
    	}
    }

    /*
    	LOSER PART

    	Show me the money
    */
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
    			HNotice_PBak(player);
    		}

    		// Gwang-Bak
    		if(hasGwan && player->gwanDeck->size == 0)
    		{
    			xRate *= 2;
    			HNotice_GBak(player);
    		}

    		/*
    			ABOUT MONEY TRANSFER

				i)  They have enough money       -> Show me the money
				ii) They don't have enough money -> Kick Out
    		*/
    		if(player->money > xRate*money_base)
    		{
    			// Can Afford to Pay
    			win_player->money += xRate*money_base;
    			player->money -= xRate*money_base;

    			HNotice_Lose(player, xRate*money_base);
    		}
    		else
    		{
    			// Cannot Afford to Pay
    			win_player->money += player->money; // Their all possesion
    			player->money = 0;
    			gameover = true;

    			HNotice_Bankrupt(player);
    		}
    	}
    }

    HGame_setTurn(game, winner);

    return gameover; // Whether Bankrupt or not
}