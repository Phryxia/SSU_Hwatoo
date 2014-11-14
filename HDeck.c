#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "HDeck.h"
#include "Debug_Center.h"

/*
	HSlot Function
*/
HSlot *new_HSlot(void)
{
	// Make 
	HSlot *temp = (HSlot *)malloc(sizeof(HSlot));
#ifdef DEBUG
	if(temp == NULL)
	{
		printError("HDeck", "Error", "new_HSlot(void)", "Allocation Fail!!");
	}
#endif
	temp->prev  = NULL;
	temp->next  = NULL;
	temp->data  = NULL;
	
	return temp;
}

/*
	HDeck Function
*/
HDeck *new_HDeck(void)
{
	HDeck *temp  = (HDeck *)malloc(sizeof(HDeck));
#ifdef DEBUG
	if(temp == NULL)
	{
		printError("HDeck", "Error", "new_HDeck(void)", "Allocation Fail!!");
	}
#endif
	temp->first  = NULL;
	temp->size   = 0;
	
	
	temp->get    = HDeck_get;
	
	temp->push   = HDeck_push;
	temp->pop    = HDeck_pop;
	temp->insert = HDeck_insert;
	temp->remove = HDeck_remove;
	
	temp->swap   = HDeck_swap;
	temp->shake  = HDeck_shake;
	temp->sort   = HDeck_sort;
	temp->clear  = HDeck_clear;
	
	temp->drawFrom = HDeck_drawFrom;
	temp->import = HDeck_import;
	
	temp->print  = HDeck_print;
	
	return temp;
}

void delete_HDeck(HDeck *me)
{
	me->clear(me);
	free(me);
}

void HDeck_shake(HDeck *me)
{
#ifdef DEBUG
	if(me == NULL)
	{
		printError("HDeck", "Error", "shake(void)", "NULL HDeck Pointer Exception!!");
	}
#endif
	if(me->size >= 3)
	{
		for(int i=0; i<(me->size)/2; ++i) // Random Factor. Expect every cards will blend.
		{
			// Select 2 card randomly and Shake it Shake it!
			int x = rand()%(me->size);
			int y;
			while((y = rand()%(me->size)) == x); // Protect Duplication
		
			me->swap(me, x, y);
		}
	}
}

HSlot *HDeck_get(HDeck *me, int pos)
{
#ifdef DEBUG
	if(me == NULL)
	{
		printError("HDeck", "Error", "get(HDeck *, int)", "NULL HDeck Pointer Exception!!");
	}
#endif

	// Boundary Protection
	if(pos < 0 || pos >= me->size)
	{
#ifdef DEBUG
		printError("HDeck", "Warning", "get(HDeck *, int)", "Wrong Position Access!!");
#endif
		return NULL;
	}
	else
	{
		HSlot *temp = me->first;
		for(int i=0; i<pos; ++i)
		{
			temp = temp->next;
		}
		return temp;
	}
}

void HDeck_push(HDeck *me, HCard *target)
{
#ifdef DEBUG
	if(me == NULL)
	{
		printError("HDeck", "Error", "push(HDeck *, HCard *)", "NULL HDeck Pointer Exception!!");
	}
	if(target == NULL)
	{
		printError("HDeck", "Error", "push(HDeck *, HCard *)", "NULL HCard Pointer Exception!!");
	}
#endif
	HSlot *new_slot = new_HSlot();
	new_slot->data = target;
		
	if(me->size == 0)
	{
		/*
			Patch Connection
			
			change
				NULL
			to
				new_slot  <-->  new_slot  <-->  new_slot
		*/
		new_slot->next = new_slot;
		new_slot->prev = new_slot;
		
		me->first = new_slot;
	}
	else
	{
		/*
			Patch Connection
			
			change
				size-1th slot  <-->  0th slot
			to
				size-1th slot  <-->  new_slot  <-->  0th slot
		*/
		
		// Search Insertion Address
		HSlot *next_slot = me->first;
		HSlot *prev_slot = next_slot->prev;
		
		// Modify Connection
		new_slot->next  = next_slot;
		new_slot->prev  = prev_slot;
		
		prev_slot->next = new_slot;
		next_slot->prev = new_slot;
	}
	
	++(me->size);
}

void HDeck_pop(HDeck *me)
{
#ifdef DEBUG
	if(me == NULL)
	{
		printError("HDeck", "Error", "pop(HDeck *)", "NULL HDeck Pointer Exception!!");
	}
#endif
	// Boundary Protection
	if(me->size > 0)
	{
		if(me->size == 1)
		{
			/*
				Patch Connection
				
				change
					this_slot  <-->  this_slot  <-->  this_slot
				to
					NULL
			*/
			
			// Search
			HSlot *this_slot = me->first;
			
			// KILL
			free(this_slot);
			
			// Patch
			me->first = NULL;
		}
		else
		{
			/*
				Patch Connection
				
				change
					prev_slot  <-->  last_slot  <-->  0th slot
				to
					prev_slot  <-->  0th slot
			*/
			
			// Search Remove Address
			HSlot *this_slot = me->first->prev;
			HSlot *next_slot = this_slot->next;
			HSlot *prev_slot = this_slot->prev;
			
			// Kill Current Slot
			free(this_slot);
			
			// Re-Patch
			prev_slot->next = next_slot;
			next_slot->prev = prev_slot;
		}
	
		--(me->size);
	}
}

void HDeck_insert(HDeck *me, HCard *target, int pos)
{
#ifdef DEBUG
	if(me == NULL)
	{
		printError("HDeck", "Error", "insert(HDeck *, HCard *, int)", "NULL HDeck Pointer Exception!!");
	}
	if(target == NULL)
	{
		printError("HDeck", "Error", "insert(HDeck *, HCard *, int)", "NULL HCard Pointer Exception!!");
	}
#endif
	// New Slot will have the position as <pos>
	// Boundary Protection
	if(pos < 0 || pos > me->size)
	{
#ifdef DEBUG
		printError("HDeck", "Warning", "insert(HDeck *, HCard *, int)", "Wrong Position Access!!");
#endif
		return ;
	}
	else
	{
		if(me->size == pos)
		{
			me->push(me, target);
		}
		else
		{
			// Assign New Card
			HSlot *new_slot = new_HSlot();
			new_slot->data  = target;
		
			/*
				Patch Connection
				
				change
					prev_slot  <-->  next_slot
				to
					prev_slot  <-->  new_slot  <-->  next_slot
			*/
			
			// Search Insertion Address
			HSlot *next_slot = me->get(me, pos);
			HSlot *prev_slot = next_slot->prev;
			
			// Modify Connection
			new_slot->next  = next_slot;
			new_slot->prev  = prev_slot;
			
			prev_slot->next = new_slot;
			next_slot->prev = new_slot;
		
			++(me->size);
			
			// Change First
			if(pos == 0)
			{
				me->first = new_slot;
			}
		}
	}
}

// I Love C Language

void HDeck_remove(HDeck *me, int pos)
{
#ifdef DEBUG
	if(me == NULL)
	{
		printError("HDeck", "Error", "remove(HDeck *, HCard *, int)", "NULL HDeck Pointer Exception!!");
	}
#endif
	// Boundary Protection
	if(pos < 0 || pos >= me->size || me->size <= 0)
	{
#ifdef DEBUG
		printError("HDeck", "Warning", "remove(HDeck *, int)", "Wrong Position Access!!");
#endif
		return ;
	}
	else
	{
		if(me->size == 1)
		{
			/*
				Patch Connection
				
				change
					this_slot  <-->  this_slot  <-->  this_slot
				to
					NULL
			*/
			
			// Search
			HSlot *this_slot = me->first;
			
			// KILL
			free(this_slot);
			
			// Patch
			me->first = NULL;
		}
		else
		{
			/*
				Patch Connection
				
				change
					prev_slot  <-->  this_slot  <-->  next_slot
				to
					prev_slot  <-->  next_slot	
			*/
			
			// Search Remove Address
			HSlot *this_slot = me->get(me, pos);
			HSlot *next_slot = this_slot->next;
			HSlot *prev_slot = this_slot->prev;
			
			// Kill Current Slot
			free(this_slot);
			
			// Re-Patch
			prev_slot->next = next_slot;
			next_slot->prev = prev_slot;
			
			// Change First Node
			if(pos == 0)
			{
				me->first = next_slot;
			}
		}
		
		--(me->size);
	}
}

void HDeck_swap(HDeck *me, int pos_1, int pos_2)
{
#ifdef DEBUG
	if(me == NULL)
	{
		printError("HDeck", "Error", "swap(HDeck *, int, int)", "NULL HDeck Pointer Exception!!");
	}
#endif
	// No Swap
	if(pos_1 == pos_2 || me->size <= 2)
	{
		return ;
	}
	else
	{
		// Arrange pos_1 and pos_2 as increase order
		if(pos_1 > pos_2)
		{
			int temp = pos_1;
			pos_1 = pos_2;
			pos_2 = temp;
		}
		
		// Search for Object's Address
		HSlot *pos_1_slot = me->get(me, pos_1);
		HSlot *pos_2_slot = me->get(me, pos_2);

#ifdef DEBUG
		if(pos_1_slot == NULL)
		{
			printError("HDeck", "Error", "swap(HDeck *, int, int)", "pos 1 is NULL");
		}
		if(pos_2_slot == NULL)
		{
			printError("HDeck", "Error", "swap(HDeck *, int, int)", "pos 2 is NULL");
		}
#endif
		
		HCard *temp = pos_1_slot->data;
		pos_1_slot->data = pos_2_slot->data;
		pos_2_slot->data = temp;
	}
}

int comp(HCard *x, HCard *y)
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

void HDeck_sort(HDeck *me)
{
	for(int i=1; i<me->size; ++i)
	{
		// Search Insertion Point
		int current_pos = i-1;
		HSlot *source  = me->get(me, i);
		HSlot *current = source->prev;
		
		while(current_pos >= 0)
		{
			if(comp(source->data, current->data) >= 0)
			{
				break;
			}
			
			current = current->prev;
			--current_pos;
		}
		
		// Swap
		me->insert(me, source->data, current_pos+1);
		me->remove(me, i+1);
	}
}

void HDeck_clear(HDeck *me)
{
#ifdef DEBUG
	if(me == NULL)
	{
		printError("HDeck", "Error", "clear(HDeck *)", "NULL HDeck Pointer Exception!!");
	}
#endif
	if(me->size > 0)
	{
		while(me->size > 0)
		{
			me->pop(me);
		}
	}
}

void HDeck_drawFrom(HDeck *me, HDeck *you, int pos)
{
#ifdef DEBUG
	if(me == NULL)
	{
		printError("HDeck", "Error", "clear(HDeck *)", "NULL HDeck Pointer Exception!!");
	}
	if(you == NULL)
	{
		printError("HDeck", "Error", "clear(HDeck *)", "NULL HDeck Pointer Exception(you)!!");
	}
#endif
	HSlot *target = you->get(you, pos); // 목표는 누구?
	
	me->push(me, target->data);
	
	you->remove(you, pos);
}

void HDeck_import(HDeck *me, HCard *CARD_SET)
{
#ifdef DEBUG
	if(me == NULL)
	{
		printError("HDeck", "Error", "import(HDeck *, HCard *)", "NULL HDeck Pointer Exception!!");
	}
	if(CARD_SET == NULL)
	{
		printError("HDeck", "Error", "import(HDeck *, HCard *)", "NULL ingredient Exception!!");
	}
#endif
	for(int i=0; i<48; ++i)
	{
		me->push(me, &CARD_SET[i]);
	}
}

void HDeck_print(HDeck *me)
{
#ifdef DEBUG
	if(me == NULL)
	{
		printError("HDeck", "Error", "print(HDeck *)", "NULL HDeck Pointer Exception!!");
	}
#endif
	if(me->size == 0)
	{
		printf("[HDeck : Note] There is no slot available in %p\n", me);
	}
	else
	{
		printf("[HDeck : Note] %p's available slots(%d)\n", me, me->size);
		for(int i=0; i<me->size; ++i)
		{
			hprint(me->get(me, i)->data);
			printf("\n");
		}
	}
}