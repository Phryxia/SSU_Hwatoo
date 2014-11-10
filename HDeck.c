#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "HDeck.h"

/*
	HSlot Function
*/
HSlot *new_HSlot(void)
{
	// Make 
	HSlot *temp = (HSlot *)malloc(sizeof(HSlot));
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
	temp->first  = NULL;
	temp->size   = 0;
	
	temp->shake  = HDeck_shake;
	temp->get    = HDeck_get;
	
	temp->push   = HDeck_push;
	temp->pop    = HDeck_pop;
	temp->insert = HDeck_insert;
	temp->remove = HDeck_remove;
	temp->swap   = HDeck_swap;
	temp->clear  = HDeck_clear;
	temp->print  = HDeck_print;
	
	return temp;
}

void free_HDeck(HDeck *me)
{
	me->clear(me);
	free(me);
}

void HDeck_shake(HDeck *me)
{
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
	// Boundary Protection
	if(pos < 0 || pos >= me->size)
	{
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
	// New Slot will have the position as <pos>
	// Boundary Protection
	if(pos < 0 || pos >= me->size)
	{
		return ;
	}
	else
	{
		// Assign New Card
		HSlot *new_slot = new_HSlot();
		new_slot->data = target;
		
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
	}
}

// I Love C Language

void HDeck_remove(HDeck *me, int pos)
{
	// Boundary Protection
	if(pos < 0 || pos >= me->size || me->size <= 0)
	{
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
		}
		
		--(me->size);
	}
}

void HDeck_swap(HDeck *me, int pos_1, int pos_2)
{
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
		
		// Save Link
		HSlot *pos_1_prev = pos_1_slot->prev;
		HSlot *pos_1_next = pos_1_slot->next;
		HSlot *pos_2_prev = pos_2_slot->prev;
		HSlot *pos_2_next = pos_2_slot->next;
		
		// Change Link
		pos_1_prev->next  = pos_2_slot;
		pos_1_next->prev  = pos_2_slot;
		pos_2_prev->next  = pos_1_slot;
		pos_2_next->prev  = pos_1_slot;
		
		pos_1_slot->next  = pos_2_next;
		pos_1_slot->prev  = pos_2_prev;
		pos_2_slot->next  = pos_1_next;
		pos_2_slot->prev  = pos_1_prev;
		
		// Special Case. When pos_1 and pos_2 is adjacent, then link between pos_1 and pos_2 will break.
		if(pos_2 - pos_1 == 1)
		{
			pos_2_slot->next = pos_1_slot;
			pos_1_slot->prev = pos_2_slot;
		}
		else if(pos_1 == 0 && pos_2 == me->size-1)
		{
			pos_1_slot->next = pos_2_slot;
			pos_2_slot->prev = pos_1_slot;
		}
		
		// If pos_1 == 0 then start position should be changed as pos_2
		if(pos_1 == 0)
		{
			me->first = pos_2_slot;
		}
	}
}

void HDeck_clear(HDeck *me)
{
	if(me->size > 0)
	{
		while(me->size > 0)
		{
			me->pop(me);
		}
	}
}

void HDeck_print(HDeck *me)
{
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