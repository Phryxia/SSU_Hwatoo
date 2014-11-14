#include <stdio.h>

void game(void);
void eat(void);
void score(void);
void stillcard(void);
void stop(void);

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

void game(void)//전체게임함수
{
    //인터페이스 : e(exit), b(잔고), h(키설명),save(저장).load

    //eat함수 

    //상태갱신

    //점수산출score함수

    //났나? - 났을 때 고 혹은 스톱

    //스톱시 stop함수 

    //턴 넘기기 
}

void eat(void)//자기턴진행함수
{
    //먹을거 있니(1~7)-뭐먹을래(1~2) 

    //까기  

    //쌋냐 

    //따악-stillcard

    //싼거먹기-stillcrad 
}

void score(void)//점수산출함수
{
    //인자에 점수산출을 넣거나 점수계산

    //광점수 계산

    //십 점수 계산

    //띠 점수 계산

    //피 점수 계산

    //더한값을 리턴
}

void stillcard(void)//상대피 뺏어오기 함수
{
    //상대피에 피가 있나?

    //상대피에 쌍피만 있는가?

    //쌍피만 있다면 쌍피를 가져온다

    //쌍피만 있는게 아니라면 그냥 피를 가져온다

    //종료
}

void stop(void)
{
    //player1이 났을 시 쓰리고, 포고, 흔들기를 고려해 점수를 곱하여 보낸다

    //플레이어2에게 보낸다

    //플레이어 2의 광박 , 피박, 멍텅구리 여부를 파악해 곱한다.

    //돈으로 환산해 플레이어 1에게 보낸다.

    //만약 파산했을 경우, 0까지만 뺀 뒤에 빠진만큼 플레이어1에게 더하고 게임을 종료한다.

    //플레이어 3에게 플레이어1의 점수를 보낸다.

    //플레이어 3의 광박, 피박, 멍텅구리 여부를 파악해 곱한다.

    //돈으로 환산해 플레이어 1에게 보낸다.

    //만약 파산했을 경우, 0까지만 뺀 뒤에 빠진만큼 플레이어1에게 더하고 게임을 종료한다.

    //패를 섞은 후 다시 시작한다.
}