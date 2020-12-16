#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <string.h>
#include "pthread.h"


#define X1 19
#define X2 20
#define BUFFER_SIZE 4
#define OVER (-1)
int y1=0,y2=0;
int ch;

void polt(){
    clear();
    mvaddstr(y1,X1,"#");
    mvaddstr(y2,X2,"*");
    ch = getch();
}

struct prodcons{
    int buffer[BUFFER_SIZE];
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

void put(struct prodcons *b,int data){
    pthread_mutex_lock(&b->lock);
    while((b->writepos+1)%BUFFER_SIZE==b->readpos){
        //printf("wait for not full\n");
        pthread_cond_wait(&b->notfull,&b->lock);
    }
    b->buffer[b->writepos]=data;
    ++b->writepos;
    if(b->writepos>=BUFFER_SIZE) b->writepos=0;
    pthread_cond_signal(&b->notempty);
    pthread_mutex_unlock(&b->lock);
}

int get(struct prodcons *b){
    int data;
    pthread_mutex_lock(&b->lock);
    while(b->writepos==b->readpos){
        //printf("wait for not empty\n");
        pthread_cond_wait(&b->notempty,&b->lock);
    }
    data = b->buffer[b->readpos];
    ++b->readpos;
    if(b->readpos>=BUFFER_SIZE) b->readpos=0;
    pthread_cond_signal(&b->notfull);
    pthread_mutex_unlock(&b->lock);
    return data;
}


struct prodcons buffer;

void *producer(void *data){
    int n;
    for(n = 0;n < 200;++n){
        put(&buffer,n);
        y1 = n;
        polt();
    }
    put(&buffer,OVER);
    //printf("producer stopped!!\n");
    return NULL;
}

void *consumer(void *data){
    int d;
    while(1){
        d = get(&buffer);
        if(d==OVER)break;
        y2 = d;
        polt();
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
