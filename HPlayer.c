#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "HCard.h"
#include "HDeck.h"
#include "HPlayer.h"
#include "Debug_Center.h"

HPlayer *new_HPlayer(void)
{
	HPlayer *me = (HPlayer *)malloc(sizeof(HPlayer));
#ifdef DEBUG
	if(me == NULL)
	{
		printError("HPlayer", "Error", "new_HPlayer(void)", "Allocation Fail!!");
	}
#endif
	
	me->name[0]     = '\0';
	me->money       = 0;
	me->myDeck      = new_HDeck();
	me->how_many_go = 0;
	me->score       = 0;
	me->score_norm  = 0;
	me->score_anim  = 0;
	me->score_line  = 0;
	me->score_gwan  = 0;
	
	me->setName     = HPlayer_setName;
	
	return me;
}

void delete_HPlayer(HPlayer *me)
{
	delete_HDeck(me->myDeck); // Because this contains several dynamic data.
	free(me);
}

void HPlayer_setName(HPlayer *me, char const *_name)
{
#ifdef DEBUG
	if(me == NULL)
	{
		printError("HPlayer", "Error", "setName(HPlayer *, char const *)", "NULL HPlayer Pointer Exception!!");
	}
#endif
	strncpy(me->name, _name, 31);
}