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
	
	// Protection
	if(temp == NULL)
	{
 #ifdef DEBUG
		printError("HDeck", "Error", "new_HSlot(void)", "Allocation Fail!!");
 #endif
		return NULL;
	}
	else
	{
		// Data Reset
		temp->prev  = NULL;
		temp->next  = NULL;
		temp->data  = NULL;
		
		return temp;
	}
}

void delete_HSlot(HSlot *me)
{
	free(me);
}

/*
	HDeck Function
*/
HDeck *new_HDeck(void)
{
	// Try to allocate memory
	HDeck *temp  = (HDeck *)malloc(sizeof(HDeck));
	
	// Allocation Fail
	if(temp == NULL)
	{
 #ifdef DEBUG
		printError("HDeck", "Error", "new_HDeck(void)", "Allocation Fail!!");
 #endif
		return NULL;
	}
	else
	{
		// Member Variable Initialize
		temp->first  = NULL;
		temp->size   = 0;
	
		return temp;
	}
}

void delete_HDeck(HDeck *me)
{
	HDeck_clear(me);
	free(me);
}

HSlot *HDeck_get(HDeck *me, int pos)
{
	// Protection
	if(me == NULL)
	{
 #ifdef DEBUG
		printError("HDeck", "Error", "get(HDeck *, int)", "NULL HDeck Pointer Exception!!");
 #endif
	}
	else
	{
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

			if(temp == NULL)
			{
 #ifdef DEBUG
				printError("HDeck", "Error", "get(HDeck *, int)", "->first is null but size is not 0");
 #endif
				return NULL;
			}
			else
			{
				// Move pos times
				for(int i=0; i<pos; ++i)
				{
					temp = temp->next;
				}
				return temp;
			}
		}
	}
}

void HDeck_push(HDeck *me, HCard const *target)
{
	if(me == NULL)
	{
 #ifdef DEBUG
		printError("HDeck", "Error", "push(HDeck *, HCard *)", "NULL HDeck Pointer Exception!!");
 #endif
		return ;
	}
	else if(target == NULL)
	{
 #ifdef DEBUG
		printError("HDeck", "Error", "push(HDeck *, HCard *)", "NULL HCard Pointer Exception!!");
 #endif
		return ;
	}
	else
	{
		// Allocation New One
		HSlot *new_slot = new_HSlot();

		if(new_slot == NULL)
		{
 #ifdef DEBUG
			printError("HDeck", "Error", "push(HDeck *, HCard *)", "New Slot Allocation Fail");
 #endif
			return ;
		}
		
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
}

void HDeck_pop(HDeck *me)
{
	// Protection
	if(me == NULL)
	{
 #ifdef DEBUG
		printError("HDeck", "Error", "pop(HDeck *)", "NULL HDeck Pointer Exception!!");
 #endif
		return ;
	}
	else
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

				if(this_slot == NULL)
				{
 #ifdef DEBUG
					printError("HDeck", "Error", "pop(HDeck *)", "->first is null but size is not 0");
 #endif
					return ;
				}
				else
				{
					// KILL
					delete_HSlot(this_slot);
				
					// Patch
					me->first = NULL;
				}
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
				delete_HSlot(this_slot);
				
				// Re-Patch
				prev_slot->next = next_slot;
				next_slot->prev = prev_slot;
			}
		
			--(me->size);
		}
	}
}

void HDeck_insert(HDeck *me, HCard const *target, int pos)
{
	if(me == NULL)
	{
 #ifdef DEBUG
		printError("HDeck", "Error", "insert(HDeck *, HCard *, int)", "NULL HDeck Pointer Exception!!");
 #endif
	}
	else if(target == NULL)
	{
 #ifdef DEBUG
		printError("HDeck", "Error", "insert(HDeck *, HCard *, int)", "NULL HCard Pointer Exception!!");
 #endif
	}
	else
	{
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
			if(pos == me->size)
			{
				HDeck_push(me, target);
			}
			else
			{
				// Assign New Card
				HSlot *new_slot = new_HSlot();
				if(new_slot == NULL)
				{
 #ifdef DEBUG
					printError("HDeck", "Error", "insert(HDeck *, HCard *, int)", "New HSlot Allocation Fail");
 #endif
				}
				else
				{
					new_slot->data = target;
				
					/*
						Patch Connection
						
						change
							prev_slot  <-->  next_slot
						to
							prev_slot  <-->  new_slot  <-->  next_slot
					*/
					
					// Search Insertion Address
					HSlot *next_slot = HDeck_get(me, pos);
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
	}
}

// I Love C Language

void HDeck_remove(HDeck *me, int pos)
{

	if(me == NULL)
	{
 #ifdef DEBUG
		printError("HDeck", "Error", "remove(HDeck *, HCard *, int)", "NULL HDeck Pointer Exception!!");
 #endif
	}
	else
	{
		// Boundary Protection
		if(pos < 0 || pos >= me->size || me->size <= 0)
		{
 #ifdef DEBUG
			printError("HDeck", "Warning", "remove(HDeck *, int)", "Wrong Position Access!!");
 #endif
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
				if(this_slot == NULL)
				{
 #ifdef DEBUG
					printError("HDeck", "Error", "remove(HDeck *, int)", "->first is null but size is not 0");
 #endif
				}
				else
				{
					delete_HSlot(this_slot);
					me->first = NULL;
					--(me->size);
				}
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
				HSlot *this_slot = HDeck_get(me, pos);
				if(this_slot == NULL)
				{
 #ifdef DEBUG
					printError("HDeck", "Error", "remove(HDeck *, int)", "->first is null but size is not 0");
 #endif
				}
				else
				{
					HSlot *next_slot = this_slot->next;
					HSlot *prev_slot = this_slot->prev;
					
					if(next_slot == NULL)
					{
 #ifdef DEBUG
						printError("HDeck", "Error", "remove(HDeck *, int)", "this_slot->next is null. What the hack you did");
 #endif
					}
					else if(prev_slot == NULL)
					{
 #ifdef DEBUG
						printError("HDeck", "Error", "remove(HDeck *, int)", "this_slot->prev is null. What the hack you did");
 #endif
					}
					else
					{
						// Kill Current Slot
						delete_HSlot(this_slot);
						
						// Re-Patch
						prev_slot->next = next_slot;
						next_slot->prev = prev_slot;
						
						--(me->size);
						
						// Change First Node
						if(pos == 0)
						{
							me->first = next_slot;
						}
					}
				}
			}
		}
	}
}

void HDeck_swap(HDeck *me, int pos_1, int pos_2)
{
	// Protection
	if(me == NULL)
	{
 #ifdef DEBUG
		printError("HDeck", "Error", "swap(HDeck *, int, int)", "NULL HDeck Pointer Exception!!");
 #endif
	}
	else
	{
		if(pos_1 != pos_2 && me->size >= 2)
		{
			// Arrange pos_1 and pos_2 as increase order for Conveinience
			if(pos_1 > pos_2)
			{
				int temp = pos_1;
				pos_1 = pos_2;
				pos_2 = temp;
			}
			
			// Search for Object's Address
			HSlot *pos_1_slot = HDeck_get(me, pos_1);
			HSlot *pos_2_slot = HDeck_get(me, pos_2);

			if(pos_1_slot == NULL)
			{
 #ifdef DEBUG
				printError("HDeck", "Error", "swap(HDeck *, int, int)", "pos 1 is NULL");
 #endif
			}
			else if(pos_2_slot == NULL)
			{
 #ifdef DEBUG
				printError("HDeck", "Error", "swap(HDeck *, int, int)", "pos 2 is NULL");
 #endif
			}
			else
			{
				HCard const *temp = pos_1_slot->data;
				pos_1_slot->data = pos_2_slot->data;
				pos_2_slot->data = temp;
			}
		}
	}
}

void HDeck_shake(HDeck *me)
{
	// Protection
	if(me == NULL)
	{
 #ifdef DEBUG
		printError("HDeck", "Error", "shake(void)", "NULL HDeck Pointer Exception!!");
 #endif
		return ;
	}
	else
	{
		if(me->size >= 2)
		{
			for(int i=0; i<(me->size)/2; ++i) // Random Factor. Expect every cards will blend.
			{
				// Select 2 card randomly and Shake it Shake it!
				int x = rand()%(me->size);
				int y;
				while((y = rand()%(me->size)) == x); // Protect Duplication
			
				HDeck_swap(me, x, y);
			}
		}
	}
}

int _Hcomp(HCard const *x, HCard const *y)
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
	if(me == NULL)
	{
 #ifdef DEBUG
		printError("HDeck", "Error", "sort(HDeck *me)", "NULL HDeck Pointer Exception");
 #endif
	}
	else
	{
		for(int i=1; i<me->size; ++i)
		{
			// Search Insertion Point
			int current_pos = i-1;
			HSlot *source  = HDeck_get(me, i);
			HSlot *current = source->prev;
			
			while(current_pos >= 0)
			{
				if(_Hcomp(source->data, current->data) >= 0)
				{
					break;
				}
				
				current = current->prev;
				--current_pos;
			}
			
			// Swap
			HDeck_insert(me, source->data, current_pos+1);
			HDeck_remove(me, i+1);
		}
	}
}

void HDeck_clear(HDeck *me)
{
	if(me == NULL)
	{
 #ifdef DEBUG
		printError("HDeck", "Error", "klear(HDeck *)", "NULL HDeck Pointer Exception!!");
 #endif
	}
	else
	{
		if(me->size > 0)
		{
			while(me->size > 0)
			{
				HDeck_pop(me);
			}
		}
	}
}

void HDeck_drawFrom(HDeck *me, HDeck *you, int pos)
{
	if(me == NULL)
	{
 #ifdef DEBUG
		printError("HDeck", "Error", "klear(HDeck *)", "NULL HDeck Pointer Exception!!");
 #endif
	}
	else if(you == NULL)
	{
 #ifdef DEBUG
		printError("HDeck", "Error", "klear(HDeck *)", "NULL HDeck Pointer Exception(you)!!");
 #endif
	}
	else
	{
		HSlot *target = HDeck_get(you, pos); // 목표는 누구?

		if(target == NULL)
		{
 #ifdef DEBUG
			printError("HDeck", "Warning", "drawFrom(HDeck *, HDeck *, int)", "NULL Target Exception : Wrong Pos");
 #endif
		}
		else
		{
			HDeck_push(me, target->data);
			HDeck_remove(you, pos);
		}
	}
}

void HDeck_import(HDeck *me, HCard const *CARD_SET)
{
	if(me == NULL)
	{
 #ifdef DEBUG
		printError("HDeck", "Error", "import(HDeck *, HCard *)", "NULL HDeck Pointer Exception!!");
 #endif
	}
	else if(CARD_SET == NULL)
	{
 #ifdef DEBUG
		printError("HDeck", "Error", "import(HDeck *, HCard *)", "NULL ingredient Exception!!");
 #endif
	}
	else
	{
		if(me->size > 0)
		{
			HDeck_clear(me);
		}
		for(int i=0; i<48; ++i)
		{
			HDeck_push(me, &CARD_SET[i]);
		}
	}
}

void HDeck_print(HDeck *me)
{
	if(me == NULL)
	{
 #ifdef DEBUG
		printError("HDeck", "Error", "print(HDeck *)", "NULL HDeck Pointer Exception!!");
 #endif
	}
	else
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
				hprint(HDeck_get(me, i)->data);
				printf("\n");
			}
		}
	}
}