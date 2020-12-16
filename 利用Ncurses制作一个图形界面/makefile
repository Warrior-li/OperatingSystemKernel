main:main.o curses.h
	gcc -o main main.o -l curses
main.o: main.c curses.h
	gcc -c -o main.o main.c
clean:
	rm main.o
