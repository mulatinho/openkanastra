all: buraco

game.o: game.c
	gcc -c game.c -o game.o

mem.o: mem.c
	gcc -c mem.c -o mem.o

main.o: main.c
	gcc -c main.c -o main.o

buraco: game.o mem.o main.o
	gcc game.o mem.o main.o -o buraco -lpthread

clean:
	rm -f *~ *.o buraco 
