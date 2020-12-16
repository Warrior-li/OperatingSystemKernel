#include <curses.h>
#include <string.h>

#define StartX 20
#define StartY 6

void initial();
void DoFork();
void Dosignal();
void DoPipe();
void DoPopen();
void DoFifo();


main(){
    int x = StartX,y = StartY,ch;
    initial();
    box(stdscr,0,0);
    attron(A_REVERSE);
    mvaddstr(0,20,"Lsx List by warriorli");
    attroff(A_REVERSE);
    mvaddstr(6,19,"[*] Fork");
    mvaddstr(8,19,"[*] Signal");
    mvaddstr(10,19,"[*] Pipe");
    mvaddstr(12,19,"[*] Popen");
    mvaddstr(14,19,"[*] Fifo");
    mvaddstr(16,19,"-----------------------");
    mvaddstr(18,19,"[*] Exit");
    move(y,x);
    do{
        ch = getch();
        switch(ch){
            case KEY_UP:
                if(y == 18)y=14;
                else if(y == 6) y = 18;
                else y = y - 2;
                break;
            case KEY_DOWN:
                if(y == 14)y=18;
                else if(y == 18) y = 6;
                else y = y+2;
                break;
            case '\r':
                if(y==6){endwin();clear();DoFork();}
    		if(y==8){endwin();clear();Dosignal();}
		if(y==10){endwin();clear();DoPipe();}
		if(y==12){endwin();clear();DoPopen();}
		if(y==14){endwin();clear();DoFifo();}
 		if(y==18){endwin();exit(1);}
                break;
            default:
                break;
        }
        move(y,x);
    }
    while(1);
}

void DoFork(){
    system("/root/2.4\\ process/1_fork/fork");
    main();
}

void Dosignal(){
    system("/root/2.4\\ process/2_signal/signal");
    main();
}

void DoPipe(){
    system("/root/2.4\\ process/3_pipe/pipe");
    main();
}

void DoPopen(){
    system("/root/2.4\\ process/4_popen/popen");
    main();
}

void DoFifo(){
    int x = StartX,y = StartY,ch;
    initial();
    box(stdscr,0,0);
    attron(A_REVERSE);
    mvaddstr(0,20,"DoFifo meun");
    attroff(A_REVERSE);
    mvaddstr(6,19,"[*] Fifo_read");
    mvaddstr(8,19,"[*] Fifo_write");
    mvaddstr(10,19,"-----------------------");
    mvaddstr(12,19,"[*] Exit");
    move(y,x);
    do{
        ch = getch();
        switch(ch){
            case KEY_UP:
                if(y == 12)y=8;
                else if(y == 6) y = 12;
                else y = y - 2;
                break;
            case KEY_DOWN:
                if(y == 8)y=12;
                else if(y == 12) y = 6;
                else y = y+2;
                break;
            case '\r':
                if(y==6){endwin();clear();Doread();}
    		if(y==8){endwin();clear();Dowrite();}
 		if(y==12){endwin();clear();main();}
                break;
            default:
                break;
        }
        move(y,x);
    }
    while(1);
}

void Doread(){
    system("/root/2.4\\ process/5_fifo/fifo1");
    DoFifo();
}

void Dowrite(){
    system("/root/2.4\\ process/5_fifo/fifo2");
    DoFifo();
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
