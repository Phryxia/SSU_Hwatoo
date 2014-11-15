#ifndef HCARD_H
#define HCARD_H
#include <stdbool.h>

/*
	HCard represent Hwa-Tu game card.
	
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
	
	Constructed By : Kwon Se Kyu
*/

// I don't like enum. enum makes codes more difficult.
#define H_NORM 0 // Pee
#define H_ANIM 1 // Animal
#define H_LINE 2 // Ddi
#define H_GWAN 3 // Gwang

// Use this like enum. (Because of sizeof(enum) == 4, and inner union should be 1.
#define HF_RED 0 // Hong-dan
#define HF_BLU 1 // Cheong-dan
#define HF_CHO 2 // Cho-dan
#define HF_ETC 3 // Just Normal DDI

typedef struct _HCard_ // Warning : Month starts as 1-Biased. Very Be Careful.
{
	int month;     // 1 ~ 12. Check Ray whether month is 11 or not. (Rain-Gwang)
	int type;      // Card Type. Look Above.
	int five_type; // Ddi Type
	bool isDouble; // Ssang Ppi
} HCard;

// Memory Control
HCard *halloc(void); // This will make 48 Hwa-Tu game cards array
void delete_HCard(HCard *card);

// Interface
bool HCard_isBird(HCard const *card);

// For Debugging
void hprint(HCard const *card);

#endif