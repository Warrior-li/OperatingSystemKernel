#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <pthread.h>
#include <errno.h>
#include <math.h>
#include <curses.h>
#include <string.h>
//筷子作为mutex
pthread_mutex_t chopstick[7] ;
int disp[10];

int mission = 200;
int x[10]={19,19,24,29,29,24};
int y[10]={10,14,18,14,10,6};
char s[10];
int ch;

void change(){
    s[0] = '0'+mission/100;
    s[1] = '0'+mission/10%10;
    s[2] = '0'+mission%10;
    s[3] ='\0';
}


void display(){
    int i;
    for(i = 1;i<=6;++i){
        if(disp[i] == 0)mvaddstr(y[i-1],x[i-1],"*");
        else mvaddstr(y[i-1],x[i-1],"#");
    }
    change();
    mvaddstr(14,24,s);
    refresh();
    return;
}


void initial(){
    initscr();
    cbreak();
    nonl();
    noecho();
    intrflush(stdscr,FALSE);
    keypad(stdscr,TRUE);
    refresh();
    return;
}


void *eat_think(void *arg)
{
    char phi = *(char *)arg;
    int left,right; //左右筷子的编号
    int count;
    switch (phi){
        case 'A':
            left = 6;
            right = 1;
            count = 1;
            break;
        case 'B':
            left = 1;
            right = 2;
            count = 2;
            break;
        case 'C':
            left = 2;
            right = 3;
            count = 3;
            break;
        case 'D':
            left = 3;
            right = 4;
            count = 4;
            break;
        case 'E':
            left = 4;
            right = 5;
            count = 5;
            break;
        case 'F':
            left = 5;
            right = 6;
            count = 6;
            break;
    }

    int i;
    for(;;){
        if(mission<=0)break;
        sleep(1);
        pthread_mutex_lock(&chopstick[left]); //拿起左手的筷子
//        printf("Philosopher %c fetches chopstick %d\n", phi, left);
        if (pthread_mutex_trylock(&chopstick[right]) == EBUSY){ //拿起右手的筷子
            pthread_mutex_unlock(&chopstick[left]); //如果右边筷子被拿走放下左手的筷子
            continue;
        }

        //	pthread_mutex_lock(&chopstick[right]); //拿起右手的筷子，如果想观察死锁，把上一句if注释掉，再把这一句的注释去掉
//        printf("Philosopher %c fetches chopstick %d\n", phi, right);
//        printf("Philosopher %c is eating.\n",phi);
        disp[count] = 1;
        display();
        --mission;
        sleep(1); //吃饭
        pthread_mutex_unlock(&chopstick[left]); //放下左手的筷子
//        printf("Philosopher %c release chopstick %d\n", phi, left);
        pthread_mutex_unlock(&chopstick[right]); //放下左手的筷子
//        printf("Philosopher %c release chopstick %d\n", phi, right);
        disp[count] = 0;
        display();
    }
}



int main(){
    pthread_t A,B,C,D,E,F; //5个哲学家
    initial();
    int i;
    for (i = 0; i < 6; i++)
        pthread_mutex_init(&chopstick[i],NULL);
    for (i = 1;i <= 6;++i)disp[i] = 0;
    display();
    pthread_create(&A,NULL, eat_think, "A");
    pthread_create(&B,NULL, eat_think, "B");
    pthread_create(&C,NULL, eat_think, "C");
    pthread_create(&D,NULL, eat_think, "D");
    pthread_create(&E,NULL, eat_think, "E");
    pthread_create(&F,NULL ,eat_think, "F");

    pthread_join(A,NULL);
    pthread_join(B,NULL);
    pthread_join(C,NULL);
    pthread_join(D,NULL);
    pthread_join(E,NULL);
    pthread_join(F,NULL);
    while(1){

    }
    return 0;
}
