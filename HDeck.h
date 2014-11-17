#ifndef HDECK_H
#define HDECK_H
#include <stddef.h>
#include "HCard.h"

/*
	HSlot Structure is equivalent to "Node".
	
	This doesn't copy the original card spirit.
	Instead, they hold card's address.
	Be careful with using these address.
*/
typedef struct _HSlot_
{
	// Member Variable
	struct _HSlot_ *prev;
	struct _HSlot_ *next;
	HCard const *data;
	
	// Member Function
} HSlot;

HSlot *new_HSlot(void);
void delete_HSlot(HSlot *me);

/*
	HDeck Structure is equivalent to "Double LinkedList".
	
	* You have to use them as 0-biased index.
	* about insert : New Slot will have input index.
					 This means that older one will be shifted.
	
	Construction by : Kwon Se Kyu
*/
typedef struct _HDeck_
{
	/*
		Member Variable
	*/
	HSlot *first;
	size_t size;
	
	/*
		Member Function
	*/
} HDeck;

/*
	Constructor & Destructor
*/
HDeck *new_HDeck(void);
void   delete_HDeck(HDeck *me);

/*
	Member Function Origin : Don't use them directly.
*/
HSlot *HDeck_get(HDeck *me, int pos);  // get N-th HSlot's Pointer
void   HDeck_push(HDeck *me, HCard const *target);
void   HDeck_pop(HDeck *me);
void   HDeck_insert(HDeck *me, HCard const *target, int pos); // 0 <= pos < size. If you want to put data, use push.
void   HDeck_remove(HDeck *me, int pos);

void   HDeck_swap(HDeck *me, int pos_1, int pos_2);
void   HDeck_shake(HDeck *me);           // This will shake holding cards.
void   HDeck_sort(HDeck *me); // Using 
void   HDeck_clear(HDeck *me); // Reset Deck but not de-allocate.

void   HDeck_drawFrom(HDeck *me, HDeck *you, int pos); // Move a card from one deck to this.
void   HDeck_import(HDeck *me, HCard const *CARD_SET);

void   HDeck_print(HDeck *me); // For Debug

#endif