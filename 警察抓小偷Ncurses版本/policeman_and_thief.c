#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <string.h>
#include "pthread.h"


#define X1 4
#define X2 6
#define OVER (-1)
int y1=0,y2=0;
int ch;

void polt(){
    clear();
    mvaddstr(X1,y1,"#");
    mvaddstr(X2,y2,"*");
    refresh();
    sleep(1);
}

struct prodcons{
    pthread_mutex_t lock;
    int readpos,writepos;
    pthread_cond_t notempty;
    pthread_cond_t notfull;
};

void init(struct prodcons *b){
    pthread_mutex_init(&b->lock,NULL);
    pthread_cond_init(&b->notempty,NULL);
    pthread_cond_init(&b->notfull,NULL);
    b->readpos=0;
    b->writepos=0;
}

void put(struct prodcons *b){
    pthread_mutex_lock(&b->lock);
    while(b->writepos-b->readpos>3){
        //printf("wait for not full\n");
        pthread_cond_wait(&b->notfull,&b->lock);
    }
    ++y1;
    ++b->writepos;
    polt();
    pthread_cond_signal(&b->notempty);
    pthread_mutex_unlock(&b->lock);
}

void get(struct prodcons *b){
    pthread_mutex_lock(&b->lock);
    while(b->readpos-b->writepos>3){
        //printf("wait for not empty\n");
        pthread_cond_wait(&b->notempty,&b->lock);
    }
    ++b->readpos;
    ++y2;
    polt();
    pthread_cond_signal(&b->notfull);
    pthread_mutex_unlock(&b->lock);
}


struct prodcons buffer;

void *producer(void *data){
    int n;
    for(n = 0;n < 200;++n){
        put(&buffer);
    }
    //printf("producer stopped!!\n");
    return NULL;
}

void *consumer(void *data){
    int n;
    for(n = 0;n < 200;++n){
        get(&buffer);
    }
    //printf("consumer stopped!!\n");
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

int main(void){
    initial();
    pthread_t ptha,pthb;
    void *retval;
    init(&buffer);
    pthread_create(&ptha,NULL,producer,0);
    pthread_create(&pthb,NULL,consumer,0);
    pthread_join(ptha,&retval);
    pthread_join(pthb,&retval);
}
