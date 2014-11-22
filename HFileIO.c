#include "HCard.h"
#include "HDeck.h"
#include "HPlayer.h"
#include "HGame.h"
#include "HFileIO.h"
#include "Debug_Center.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/*
	File Format Description

	i)   Every new Structure Type start with '>', end with '<'
	ii)  To make parsing easily, every tokens are seperated by ' '
	iii) Every last of the line is end with '\n'.
	iv)  Every comments start with '[', end with ']'
*/
/*
	SAVE FUNCTION
*/
void HFileIO_tab(FILE *oFile, int depth)
{
	/*
		Styling!!!
	*/
	for(int i=0; i<depth; ++i)
	{
		fprintf(oFile, "	");
	}
}

void HFileIO_exportDeck(FILE *oFile, HDeck *deck, HCard const *CARD_SET, char const *label, int depth)
{
	/*
		About exportDeck

		Iterate input deck and get the card number.
		Save these card number at the oFile.
		Card number is calculated by referencing CARD_SET
	*/
	HFileIO_tab(oFile, depth);
	fprintf(oFile, "> [ HDeck : %s ]\n", label);
	HFileIO_tab(oFile, depth);
	fprintf(oFile, "  [ size ] %d\n", deck->size);

	// Print Every Card Number. This Order is start from first.
	HFileIO_tab(oFile, depth);
	fprintf(oFile, "  [ card ]");
	for(int c=0; c<deck->size; ++c)
	{
		HCard const *this_card = HDeck_get(deck, c)->data;

		fprintf(oFile, " %2d", this_card-CARD_SET);
	}
	fprintf(oFile, "\n");

	// End Sequence
	HFileIO_tab(oFile, depth);
	fprintf(oFile, "<\n");
}

void HFileIO_exportPlayer(FILE *oFile, HPlayer *player, HCard const *CARD_SET, char const *label, int depth)
{
	/*
		About exportPlayer

		Export player's data & name.
		exportDeck will be exported internally.
	*/
	HFileIO_tab(oFile, depth);
	fprintf(oFile, "> [ HPlayer : %s ]\n", label);
	HFileIO_tab(oFile, depth);
	fprintf(oFile, "  [ name  ] %s\n", player->name);
	HFileIO_tab(oFile, depth);
	fprintf(oFile, "  [ money ] %d\n", player->money);
	HFileIO_tab(oFile, depth);
	fprintf(oFile, "  [ gonum ] %d\n", player->how_many_go);
	HFileIO_tab(oFile, depth);
	fprintf(oFile, "  [ score ] %d\n", player->score);
	HFileIO_tab(oFile, depth);
	fprintf(oFile, "  [ goscr ] %d\n", player->score_lastgo);
	
	HFileIO_tab(oFile, depth);
	fprintf(oFile, "  [ APchn ]");
	if(player->hasChangeAP)
	{
		fprintf(oFile, " 1\n");
	}
	else
	{
		fprintf(oFile, " 0\n");
	}

	HFileIO_tab(oFile, depth);
	fprintf(oFile, "  [ shake ]");
	for(int m=0; m<12; ++m)
	{
		if(player->shaked[m])
		{
			fprintf(oFile, " 1");
		}
		else
		{
			fprintf(oFile, " 0");
		}
	}
	fprintf(oFile, "\n");

	// Deck
	HFileIO_exportDeck(oFile, player->myDeck  , CARD_SET, "myDeck"  , depth+1);
	HFileIO_exportDeck(oFile, player->normDeck, CARD_SET, "normDeck", depth+1);
	HFileIO_exportDeck(oFile, player->animDeck, CARD_SET, "animDeck", depth+1);
	HFileIO_exportDeck(oFile, player->lineDeck, CARD_SET, "lineDeck", depth+1);
	HFileIO_exportDeck(oFile, player->gwanDeck, CARD_SET, "gwanDeck", depth+1);

	HFileIO_tab(oFile, depth);
	fprintf(oFile, "<\n");
}

bool HFileIO_saveGame(HGame *game, HCard const *CARD_SET)
{
	/*
		Open the File as Writing
	*/
	FILE *oFile = fopen("savedata.txt", "w");

	// Check for File Open Failure
	if(oFile == NULL)
	{
 #ifdef DEBUG
		printError("HFileIO", "Error", "saveGame", "Fail to open the savedata.txt");
 #endif
		return false;
	}

	// 
	fprintf(oFile, "> [ HGame : Main ]\n");
	fprintf(oFile, "  [ current_player_num ] %d\n", game->current_player_num);
	if(game->was_nagari)
	{
		fprintf(oFile, "  [ was_nagari ] 1\n");
	}
	else
	{
		fprintf(oFile, "  [ was_nagari ] 0\n");
	}

	// Player Export
	HFileIO_exportPlayer(oFile, game->player[0], CARD_SET, "Player_1", 1);
	HFileIO_exportPlayer(oFile, game->player[1], CARD_SET, "Player_2", 1);
	HFileIO_exportPlayer(oFile, game->player[2], CARD_SET, "Player_3", 1);

	// Deck
	HFileIO_exportDeck(oFile, game->unknown_cards, CARD_SET, "Unknown_cards", 1);
	for(int m=0; m<12; ++m)
	{
		char contents[32];
		sprintf(contents, "Visible_cards(%d)", m);
		HFileIO_exportDeck(oFile, game->visible_cards[m], CARD_SET, contents, 1);
	}
	HFileIO_exportDeck(oFile, game->display_cards, CARD_SET, "Display_cards", 1);

	fprintf(oFile, "<\n");
	fclose(oFile);

	return true;
}

/*
	LOAD FUNCTION
*/
void HFileIO_throwLine(FILE *iFile)
{
	/*
		This throw one line entirely.
	*/
	int c;
	while((c = fgetc(iFile)) != '\n' && c != EOF);
}

void HFileIO_goNextToken(FILE *iFile)
{
	/*
		This will move file pointer untill search ']' or EOF
	*/
	int c;
	while((c = fgetc(iFile)) != ']' && c != EOF);
}

void HFileIO_endThisBlock(FILE *iFile)
{
	/*
		This will move file pointer until search '<'
	*/
	int c;
	while((c = fgetc(iFile)) != '<' && c != EOF);
	HFileIO_throwLine(iFile);
}

void HFileIO_loadDeck(FILE *iFile, HDeck *deck, HCard const *CARD_SET)
{
	int size;
	HDeck_clear(deck);

 #ifdef DEBUG
	printError("HFileIO", "Note", "loadDeck", "Try to load HDeck");
 #endif

	// Start from >
	HFileIO_throwLine(iFile);   // Throw [ HDeck : ... ]
	HFileIO_goNextToken(iFile); // Go [ Size ]
	fscanf(iFile, "%d", &size);
	HFileIO_goNextToken(iFile); // Go [ Card ]

	for(int c=0; c<size; ++c)
	{
		int card_num;
		fscanf(iFile, "%d", &card_num);
		HDeck_insert(deck, &CARD_SET[card_num], 0);
	}

	HFileIO_endThisBlock(iFile); // Go <

 #ifdef DEBUG
	printError("HFileIO", "Note", "loadDeck", "Success loading HDeck");
 #endif
}

void HFileIO_loadPlayer(FILE *iFile, HPlayer *player, HCard const *CARD_SET)
{
	int temp;
	HFileIO_throwLine(iFile); // Throw [ HPlayer : ... ]
	HFileIO_goNextToken(iFile);
	fscanf(iFile, "%s", player->name);
	HFileIO_goNextToken(iFile);
	fscanf(iFile, "%d", &(player->money));
	HFileIO_goNextToken(iFile);
	fscanf(iFile, "%d", &(player->how_many_go));
	HFileIO_goNextToken(iFile);
	fscanf(iFile, "%d", &(player->score));
	HFileIO_goNextToken(iFile);
	fscanf(iFile, "%d", &(player->score_lastgo));
	HFileIO_goNextToken(iFile);
	fscanf(iFile, "%d", &temp); // APChange
	if(temp)
	{
		player->hasChangeAP = true;
	}
	else
	{
		player->hasChangeAP = false;
	}
	HFileIO_goNextToken(iFile); // Shaked
	for(int m=0; m<12; ++m)
	{
		fscanf(iFile, "%d", &temp); // APChange
		if(temp)
		{
			player->shaked[m] = true;
		}
		else
		{
			player->shaked[m] = false;
		}
	}
	fgetc(iFile); // Flush '\n'

	HFileIO_loadDeck(iFile, player->myDeck  , CARD_SET);
	HFileIO_loadDeck(iFile, player->normDeck, CARD_SET);
	HFileIO_loadDeck(iFile, player->animDeck, CARD_SET);
	HFileIO_loadDeck(iFile, player->lineDeck, CARD_SET);
	HFileIO_loadDeck(iFile, player->gwanDeck, CARD_SET);
	HFileIO_endThisBlock(iFile);
}

bool HFileIO_loadGame(HGame *game, HCard const *CARD_SET)
{
	/*
		Loading is little tricky.

		Simple Data (Non-Dynamic) loading is pretty easy.
		But, pointer-based data is pretty hard.

		As game instance loaded, you don't have to re-allocate datas.
		But reset is very important, so don't forget.
	*/
	FILE *iFile = fopen("savedata.txt", "r");

	if(iFile == NULL)
	{
 #ifdef DEBUG
		printError("HFileIO", "Error", "loadGame", "Fail Loading savedata.txt");
 #endif
		return false;
	}

	int temp;
	HFileIO_throwLine(iFile); // Throw [HGame : Main ]
	HFileIO_goNextToken(iFile);
	fscanf(iFile, "%d", &(game->current_player_num));
	HFileIO_goNextToken(iFile);
	fscanf(iFile, "%d", &temp); // Was Nagari?
	if(temp)
	{
		game->was_nagari = true;
	}
	else
	{
		game->was_nagari = false;
	}
	HFileIO_throwLine(iFile);

	HFileIO_loadPlayer(iFile, game->player[0], CARD_SET);
	HFileIO_loadPlayer(iFile, game->player[1], CARD_SET);
	HFileIO_loadPlayer(iFile, game->player[2], CARD_SET);

	HFileIO_loadDeck(iFile, game->unknown_cards, CARD_SET);
	for(int m=0; m<12; ++m)
	{
		HFileIO_loadDeck(iFile, game->visible_cards[m], CARD_SET);
	}

	HFileIO_loadDeck(iFile, game->display_cards, CARD_SET);

	HFileIO_endThisBlock(iFile);
	return true;
}