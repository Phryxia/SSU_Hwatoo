#ifndef HDECK_H
#define HDECK_H
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
	HCard *data;
	
	// Member Function
} HSlot;

HSlot *new_HCard(void);

/*
	HDeck Structure is equivalent to "Double LinkedList".
	
	Warning : You have to use them as 0-biased index.
	
	Cosntructed by : Kwon Se Kyu
*/
typedef struct _HDeck_
{
	/*
		Member Variable
	*/
	HSlot *first;
	int    size;
	
	/*
		Member Function
	*/
	void (*shake)(struct _HDeck_ *me);
	HSlot *(*get)(struct _HDeck_ *me, int pos);
	
	void (*push)(struct _HDeck_ *me, HCard *);
	void (*pop)(struct _HDeck_ *me);
	void (*insert)(struct _HDeck_ *me, HCard *, int pos);
	void (*remove)(struct _HDeck_ *me, int pos);
	void (*swap)(struct _HDeck_ *me, int pos_1, int pos_2);
	void (*clear)(struct _HDeck_ *me);
	void (*drawFrom)(struct _HDeck_ *me, struct _HDeck_ *you, int pos);
	void (*print)(struct _HDeck_ *me);
} HDeck;

HDeck *new_HDeck(void);
void   delete_HDeck(HDeck *me);

HSlot *HDeck_get(HDeck *me, int pos);  // get N-th HSlot's Pointer
void   HDeck_push(HDeck *me, HCard *target);
void   HDeck_pop(HDeck *me);
void   HDeck_insert(HDeck *me, HCard *target, int pos); // 0 <= pos < size. If you want to put data, use push.
void   HDeck_remove(HDeck *me, int pos);
void   HDeck_swap(HDeck *me, int pos_1, int pos_2);
void   HDeck_shake(HDeck *me);           // This will shake holding cards.
void   HDeck_clear(HDeck *me); // Reset Deck but not de-allocate.
void   HDeck_drawFrom(HDeck *me, HDeck *you, int pos); // Move a card from one deck to this.

void   HDeck_print(HDeck *me); // For Debug

#endif