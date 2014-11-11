#include <stdio.h>
#include "HDeck.h"
#include "HCard.h"
#include "HGame.h"
#include "Debug_Center.h"

int main(void)
{
	logReset();
	HCard *CARD_SET = halloc();
	
	// Please Test HPlayer, HGame1
	HDeck *myDeck = new_HDeck();
	
	myDeck->print(myDeck);
	
	delete_HDeck(myDeck);
	hfree(CARD_SET);
	return 0;
}