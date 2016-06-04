#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

int extractCard(int* cards,int* turnPtr);
int AIPlayCard(int* handCard,int currentSuit,int lastCard);
int playCard(int cardIndex,int* handCard);
int decideColorChange(int* HandCard,int currentSuit);

int cardScore[52];

char* suit[4]={"方片","红桃","梅花","黑桃"};
char* cardPoint[13]={"A","2","3","4","5","6","7","8","9","10","J","Q","K"};
int main(){


    /*游戏初始化
     * int[52]的数组来代表52张牌
    0-12方片,13-25红桃,26-38梅花,39-51黑桃
    方片为x/13==0,红桃1,梅花2,黑桃3
    1-K   为  x%13== 0  到12*/
    int turn=0;
    int* turnPtr=&turn;
    int cards[52];

    int cardCount=52;
    int XScore=0;
    int YScore=0;
    int XHandCard[5];
    int YHandCard[5];
    int currentSuit=0;//方片为x/13==0,红桃1,梅花2,黑桃3
    int lastCard;
    for (int i = 0; i < 52; ++i) {
        cards[i]=i;

        if((i%13)!=10){
            cardScore[i]=(i+1)%13;
        } else {
            cardScore[i]=0;
        }

        // printf("%d ",cardScore[i]) ;
    }

    /*洗牌*/
    int p,tmp;
    srand((time(0)));
    for (int i = 0; i <52; i++)
    {

        p = rand() % 52;
        tmp = cards[i];
        cards[i] = cards[p];
        cards[p] = tmp;
    }

    /*选择游戏模式*/

    int currentCard=-1;
    int currentCardIndex=-1;
    int AICurrentCard=-1;
    int AI2CurrentCard=-1;
    int AICurrentCardIndex=-1;
    int AI2CurrentCardIndex=-1;
    double winRate1=0;
    double winRate2=0;
    printf("变色龙纸牌游戏\n");
    printf("清选择游戏模式:  0 人机对战       1 机机对战\n");

    int gameMode;
    scanf("%d",&gameMode);
    /*游戏主题*/
    if(gameMode==0){


        int p,tmp;
        srand((time(0)));
        for (int i = 0; i <52; i++)
        {

            p = rand() % 52;
            tmp = cards[i];
            cards[i] = cards[p];
            cards[p] = tmp;
        }



        int humanWinCount=0;
        for (int k = 1; ;++k) {


            printf("人机模式\n");
            sleep(1);

            for (int i = 0; i < 8; ++i) {
                if (i % 2 == 0) {
                    XHandCard[i / 2] = extractCard(cards, turnPtr);
                    //   printf("%d ",XHandCard[i/2]);
                } else {
                    YHandCard[i / 2] = extractCard(cards, turnPtr);
                    //    printf("%d ",YHandCard[i/2]);
                }
            }
            printf("发初始手牌,您的手牌是\n");
            for (int i = 0; i < 4; ++i) {
                printf("%s%s ", suit[XHandCard[i] / 13], cardPoint[XHandCard[i] % 13]);
            }

            printf("\n");
            sleep(1);
            while (turn < 52) {
                /* 人的回合*/
                printf("您的回合,");
                if (turn != 8)
                    printf("当前花色是%s,", suit[currentSuit]);
                printf("抽牌\n");
                XHandCard[4] = extractCard(cards, turnPtr);
                printf("您的手牌是:");

                for (int i = 0; i < 5; ++i) {
                    printf("%d.%s%s ", i, suit[XHandCard[i] / 13], cardPoint[XHandCard[i] % 13]);
                }

                printf("\n");
                printf("选择您要出的牌\n");
                scanf("%d", &currentCardIndex);
                if (currentCardIndex > 4 || currentCardIndex < 0) {
                    printf("输入有误,请重新输入\n");
                    scanf("%d", &currentCardIndex);

                }
                printf("您打出了一张");
                printf("%s%s ", suit[XHandCard[currentCardIndex] / 13], cardPoint[XHandCard[currentCardIndex] % 13]);
                currentCard = XHandCard[currentCardIndex];
                playCard(currentCardIndex, XHandCard);
                if (turn == 9) {
                    currentSuit = currentCard / 13;
                    printf("当前花色是%s", suit[currentSuit]);

                }
                if (currentCard / 13 == currentSuit) {
                    printf("跟牌\n");
                } else if (currentCard % 13 == lastCard % 13 || currentCard % 13 == 10) {
                    printf("变色,清选择要变为的花色:       0.方片    1.红桃      2.梅花      3.黑桃");
                    scanf("%d", &currentSuit);
                    if (currentSuit > 3 || currentSuit < 0) {
                        printf("输入有误,请重新输入\n");
                        scanf("%d", &currentSuit);
                        printf("当前花色变为%s", suit[currentSuit]);

                    }
                } else {
                    printf("扣牌\n");
                    YScore += cardScore[currentCard];
                }
                lastCard = currentCard;
                printf("最后一张牌是%s%s\n", suit[lastCard / 13], cardPoint[lastCard % 13]);


                /*电脑的回合*/
                printf("电脑的回合\n");
                sleep(1);
                YHandCard[4] = extractCard(cards, turnPtr);
                AI2CurrentCardIndex = AIPlayCard(YHandCard, currentSuit, lastCard);
                AI2CurrentCard = YHandCard[AI2CurrentCardIndex];

                printf("电脑打出了一张");
                printf("%s%s \n", suit[YHandCard[AI2CurrentCardIndex] / 13],
                       cardPoint[YHandCard[AI2CurrentCardIndex] % 13]);
                playCard(AI2CurrentCardIndex, YHandCard);
                sleep(1);
                if (AI2CurrentCard / 13 == currentSuit) {
                    printf("跟牌\n");
                } else if (AI2CurrentCard % 13 == lastCard % 13 || AI2CurrentCard % 13 == 10) {
                    currentSuit = decideColorChange(YHandCard, currentSuit);
                    printf("变色,花色变为%s,", suit[currentSuit]);
                } else {
                    printf("扣牌\n");
                    XScore += cardScore[AI2CurrentCard];
                }
                lastCard = AI2CurrentCard;
                printf("最后一张牌是%s%s\n", suit[lastCard / 13], cardPoint[lastCard % 13]);


            }
            printf("游戏结束,您的得分为%d,电脑得分为%d\n", XScore, YScore);
            if (XScore > YScore) {
                humanWinCount++;
                printf("您获胜了");
            } else if (YScore > XScore) {
                printf("电脑获胜了");
            } else {
                printf("平局");
            }
            turn=0;
            XScore=0;
            YScore=0;
            winRate1=humanWinCount/(k/100);
            printf("已经进行了%d局游戏，您赢了%d局，胜率为%lf%c\n",k,humanWinCount,winRate1,'%');
        }

    } else if(gameMode==1){
        int roundCount=0;
        int p1Wincount=0;
        int p2Wincount=0;

        printf("请输入需要进行几局游戏");
        scanf("%d",&roundCount);
        for (int k = 1; k <=roundCount ; ++k) {


            int p,tmp;
            srand((time(0)));
            for (int i = 0; i <52; i++)
            {

                p = rand() % 52;
                tmp = cards[i];
                cards[i] = cards[p];
                cards[p] = tmp;
            }




            printf("机机模式");
            for (int i = 0; i < 8; ++i) {
                if(i%2==0){
                    XHandCard[i/2]=extractCard(cards,turnPtr);
                } else{
                    YHandCard[i/2]=extractCard(cards,turnPtr);
                }
            }
            while (turn<52){
                /*电脑1的回合*/
                printf("电脑1的回合\n");
             //   sleep(1);
                XHandCard[4]=extractCard(cards,turnPtr);
                if(turn!=9) {
                    AICurrentCardIndex = AIPlayCard(XHandCard, currentSuit, lastCard);
                } else {
                    AICurrentCardIndex=0;
                    AICurrentCard=XHandCard[AICurrentCardIndex];
                    currentSuit=AICurrentCard/13;



                }
                AICurrentCard=XHandCard[AICurrentCardIndex];

                printf("电脑1打出了一张");
                printf("%s%s \n",suit[XHandCard[AICurrentCardIndex]/13],cardPoint[XHandCard[AICurrentCardIndex]%13]);
                playCard(AICurrentCardIndex,XHandCard);
           //     sleep(1);
                if(AICurrentCard/13==currentSuit){
                    printf("跟牌\n");
                } else if(AICurrentCard%13==lastCard%13||AICurrentCard%13==10){
                    currentSuit=decideColorChange(XHandCard,currentSuit);
                    printf("变色,花色变为%s,",suit[currentSuit]);
                } else{
                    printf("扣牌\n");
                    YScore+=cardScore[AICurrentCard];
                }
                lastCard=AICurrentCard;
                printf("最后一张牌是%s%s\n",suit[lastCard/13],cardPoint[lastCard%13]);





                /*电脑2的回合*/
                printf("电脑2的回合\n");
         //       sleep(1);
                YHandCard[4]=extractCard(cards,turnPtr);
                AI2CurrentCardIndex=AIPlayCard(YHandCard,currentSuit,lastCard);
                AI2CurrentCard=YHandCard[AI2CurrentCardIndex];

                printf("电脑打出了一张");
                printf("%s%s \n",suit[YHandCard[AI2CurrentCardIndex]/13],cardPoint[YHandCard[AI2CurrentCardIndex]%13]);
                playCard(AI2CurrentCardIndex,YHandCard);
           //     sleep(1);
                if(AI2CurrentCard/13==currentSuit){
                    printf("跟牌\n");
                } else if(AI2CurrentCard%13==lastCard%13||AI2CurrentCard%13==10){
                    currentSuit=decideColorChange(YHandCard,currentSuit);
                    printf("变色,花色变为%s,",suit[currentSuit]);
                } else{
                    printf("扣牌\n");
                    XScore+=cardScore[AI2CurrentCard];
                }
                lastCard=AI2CurrentCard;
                printf("最后一张牌是%s%s\n",suit[lastCard/13],cardPoint[lastCard%13]);









            }
            printf("游戏结束,电脑1的得分为%d,电脑2得分为%d",XScore,YScore);
            if(XScore>YScore){
                printf("电脑1获胜了\n");
                p1Wincount++;
            } else if(YScore>XScore){
                printf("电脑2获胜了\n");
                p2Wincount++;
            } else{
                printf("平局\n");
            }
            turn=0;
            turnPtr;
            XScore=0;
            YScore=0;

            winRate1=p1Wincount/(k/100.0);
            winRate2=p2Wincount/(k/100.0);
            printf("一共进行%d局，电脑1赢了%d局，胜率%lf%c,电脑2赢了%d局，胜率%lf%c\n",roundCount,p1Wincount,winRate1,'%',p2Wincount,winRate2,'%');

        }


    }







}

int extractCard(int* cards,int* turnPtr){
    int turn=*turnPtr;
    int res=cards[turn];
    *turnPtr=turn+1;
    return  res;
}

int decideColorChange(int* HandCard,int currentSuit){
    int colorNum[4];
    int most=0;
    int color=-1;
    for (int i = 0; i < 4; ++i) {
        colorNum[i]=HandCard[i]/13;
    }
    for (int j = 0; j < 4; ++j) {
        if(colorNum[j]>most){
            color=j;
        }
    }
    return color;

}









int playCard(int cardIndex,int* handCard){


    int cardPlayed= handCard[cardIndex];
    handCard[cardIndex]=handCard[4];
    handCard[4]=-1;
    return cardPlayed;
}


int AIPlayCard(int* handCard,int currentSuit,int lastCard){
    int bestCard=-1;
    int bestCardIndex=-1;
    for (int i = 0; i < 5; ++i) {
        if(handCard[i]/13==currentSuit){
            if(cardScore[handCard[i]]>cardScore[bestCard]){
                bestCard=handCard[i];
                bestCardIndex=i;
            }
        }
    }
    if(bestCardIndex==-1){
        for (int i = 0; i < 5; ++i) {
            if(handCard[i]==lastCard){
                bestCardIndex= i;
            }
        }
    }

    if(bestCardIndex==-1){
        for (int i = 0; i < 5; ++i) {
            if(handCard[i]%13==10){
                bestCardIndex=i;
            }
        }
    }
    int min=13;
    int minIndex=-1;
    if(bestCardIndex==-1){
        for (int i = 0; i <5 ; ++i) {
            if(cardScore[handCard[i]<min]){
                minIndex=i;
            }
        }
        bestCardIndex=minIndex;
    }
    return bestCardIndex;


}






