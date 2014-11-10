#include <stdio.h>
#include "HDeck.h"
#include "HCard.h"

int main(void)
{
	HCard *CARD_SET = halloc();
	HDeck *my_list = new_HDeck();
	
	for(int i=0; i<48; ++i)
	{
		my_list->push(my_list, CARD_SET+i);
	}
	
	printf("debug\n");
	
	my_list->shake(my_list);
	
	my_list->print(my_list);
	
	free_HDeck(my_list);
	hfree(CARD_SET);
	return 0;
}