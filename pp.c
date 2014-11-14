#include <stdio.h>

void game(void);
void eat(void);
void score(void);

int main(void)
{
    int a;
    while(1)
    {
        printf("1.new game\n2.load game\n3.exit");
        scanf("%d", &a);
        switch(a)
        {
            case 1: 
                //game start
                printf("new game\n");
                break;
            case 2:
                //load game
                printf("load game\n");
                break;
            case 3:
                printf("exit\n");
                return 0;//exit
            default:
                break;//잘못된 입력 
        }
    }

    return 0;
}

void game(void)
{
    //인터페이스 : e(exit), b(잔고), h(키설명),save(저장).load

    //내기 - 흔들기 확인+ eat 

    //덱어서 까기 - eat

    //상태갱신

    //점수산출

    //났나? - 났을 때 고 혹은 스톱

    //스톱시 score 

    //턴 넘기기 
}

void eat(void)
{
    //먹을거 있니(1~7)-뭐먹을래(1~2) 
    //까기  
    //쌋냐 
    //따악-뺏어오기-상대피확인
    //싼거먹기-뺏어오기-상대피확인 
}

void score(void)
{
    //인자에 점수산출을 넣거나 점수계산

    // 
}