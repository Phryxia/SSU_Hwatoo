#include "HNotice.h"
#include "HGUI.h"
#include "Renderer.h"
#include <stdio.h>

void HNotice_FClr(HPlayer const *player)
{
	char contents[64];
	sprintf(contents, "Incredible!!! %s clean the floor!!!", player);
	Renderer_notice(contents, CARD_HEIGHT);
}

void HNotice_Shake(HPlayer const *player)
{
	char contents[32];
	sprintf(contents, "%s Shake!!!", player);
	Renderer_notice(contents, CARD_HEIGHT);
}

void HNotice_MBak(HPlayer const *player)
{
	char contents[64];
    sprintf(contents, "Wow %s, you did Meong-Teong-Gu-Ri!", player->name);
    Renderer_notice(contents, CARD_HEIGHT);
}

void HNotice_PBak(HPlayer const *player)
{
	char contents[64];
    sprintf(contents, "Player %s, you are PPI-bak!", player->name);
    Renderer_notice(contents, CARD_HEIGHT);
}

void HNotice_GBak(HPlayer const *player)
{
	char contents[64];
    sprintf(contents, "Player %s, you are GWANG-bak!", player->name);
    Renderer_notice(contents, CARD_HEIGHT);
}

void HNotice_Win(HPlayer const *player)
{
	char contents[64];
	sprintf(contents, "Good job %s! You Win!", player->name);
	Renderer_notice(contents, CARD_HEIGHT);
}

void HNotice_Lose(HPlayer const *player, int money)
{
	char contents[64];
    sprintf(contents, "Player %s, you lose %d won...", player->name, money);
    Renderer_notice(contents, CARD_HEIGHT);
}

void HNotice_Nagari(void)
{
	Renderer_notice("Congratulation! This phase is Nagari! Next phase will make x2 money.", CARD_HEIGHT);
}

void HNotice_President(HPlayer const *player)
{
	char contents[128];
	sprintf(contents, "What the... %s are president! 0~0 You win!", player->name);
	Renderer_notice(contents, CARD_HEIGHT);
}

void HNotice_Bankrupt(HPlayer const *player)
{
	char contents[128];
    sprintf(contents, "%s got financial bankrupt. So he/she called the police. You guys are arrested.", player->name);
    Renderer_notice(contents, CARD_HEIGHT);
}