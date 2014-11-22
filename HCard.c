#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include "HCard.h"
#include "Debug_Center.h"

/*
	Check this table.
	
	P : Normal, * : Double Pi
	A : Animal, ^ : Godori
	R : Red, B : Blue, C : Cho-dan, X : Just Ddi
	G : Gwang, 
	
	1  2  3  4  5  6  7  8  9  10 11 12
	G  A^ G  A^ A  A  A  G  A* A  G  G'
	R  R  R  C  C  B  C  A^ B  B  P* A
	P  P  P  P  P  P  P  P  P  P  P  X
	P  P  P  P  P  P  P  P  P  P  P  P*
	
	You can access card like this
	
	card[m*4 + n], 0 <= m < 12, 0 <= n < 4
*/

HCard *halloc(void)
{
	HCard *temp = (HCard *)malloc(sizeof(HCard)*48);
	
	if(temp == NULL)
	{
#ifdef DEBUG
		printError("HCard", "Error", "halloc(void)", "Allocation Fail!!");
#endif
		return NULL;
	}
	else
	{
		// Assign Month
		for(int i=0; i<12; ++i)
		{
			for(int j=0; j<4; ++j)
			{
				temp[i*4 + j].month     = i+1;    // 0-Biased to 1-Biased
				temp[i*4 + j].type      = H_NORM; // Initialize
				temp[i*4 + j].five_type = -1;
				temp[i*4 + j].isDouble  = false;
			}
		}
		
		// January
		temp[0].type = H_GWAN;
		temp[1].type = H_LINE;
		temp[1].five_type = HF_RED;
		
		// Feburary
		temp[4].type = H_ANIM;
		temp[5].type = H_LINE;
		temp[5].five_type = HF_RED;
		
		// March
		temp[8].type = H_GWAN;
		temp[9].type = H_LINE;
		temp[9].five_type = HF_RED;
		
		// April
		temp[12].type = H_ANIM;
		temp[13].type = H_LINE;
		temp[13].five_type = HF_CHO;
		
		// May
		temp[16].type = H_ANIM;
		temp[17].type = H_LINE;
		temp[17].five_type = HF_CHO;
		
		// June
		temp[20].type = H_ANIM;
		temp[21].type = H_LINE;
		temp[21].five_type = HF_BLU;
		
		// July
		temp[24].type = H_ANIM;
		temp[25].type = H_LINE;
		temp[25].five_type = HF_CHO;
		
		// August
		temp[28].type = H_GWAN;
		temp[29].type = H_ANIM;
		
		// September
		temp[32].type = H_ANIM;
		temp[32].isDouble = true;
		temp[33].type = H_LINE;
		temp[33].five_type = HF_BLU;
		
		// October
		temp[36].type = H_ANIM;
		temp[37].type = H_LINE;
		temp[37].five_type = HF_BLU;
		
		// November
		temp[40].type = H_GWAN;
		temp[41].isDouble = true;
		
		// December
		temp[44].type = H_GWAN;
		temp[45].type = H_ANIM;
		temp[46].type = H_LINE;
		temp[46].five_type = HF_ETC;
		
		return temp;
	}
}

void delete_HCard(HCard *card)
{
	free(card);
}

bool HCard_isBird(HCard const *card)
{
	if(card == NULL)
	{
 #ifdef DEBUG
		printError("HCard", "Error", "isBird(HCard *)", "NULL HCard Pointer Exception!!");
 #endif
	}
	else
	{
		if(card->type == H_ANIM)
		{
			switch(card->month)
			{
				case 2:
				case 4:
				case 8:
					return true;
				default:
					return false;
			}
		}
		else
		{
			return false;
		}
	}
}

int HCard_comp(HCard const *x, HCard const *y)
{
	if(x->month > y->month)
	{
		return 1;
	}
	else if(x->month < y->month)
	{
		return -1;
	}
	else
	{
		if(x->type > y->type)
		{
			return 1;
		}
		else if(x->type < y->type)
		{
			return -1;
		}
		else
		{
			if(x->five_type > y->five_type)
			{
				return 1;
			}
			else if(x->five_type < y->five_type)
			{
				return -1;
			}
			else
			{
				return 0;
			}
		}
	}
}

void hprint(HCard const *card)
{
#ifdef DEBUG
	if(card == NULL)
	{
		printError("HCard", "Error", "hprint(HCard *)", "NULL HCard Pointer Exception!!");
	}
#endif
	printf("Month : %d ", card->month);
		
	printf("Type : ");
	switch(card->type)
	{
		case H_NORM:
			if(card->isDouble)
			{
				printf("Ssang-");
			}		
			printf("Ppi ");
			break;
		case H_ANIM:
			if(HCard_isBird(card))
			{
				printf("Godori ");
			}
			else
			{
				printf("Animal ");
			}
			break;
		case H_LINE:
			printf("Ddi ");
			break;
		case H_GWAN:
			printf("Gwang ");
			break;
	}
	
	// Type 
	if(card->type == H_LINE)
	{
		switch(card->five_type)
		{
			case HF_RED:
				printf("(Hong-Dan)");
				break;
			case HF_BLU:
				printf("(Choeng-Dan)");
				break;
			case HF_CHO:
				printf("(Cho-Dan)");
				break;
			case HF_ETC:
				printf("(Just Dan)");
				break;
			default:
				break;
		}
	}
}

