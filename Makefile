all: buraco

game.o: game.c
	gcc -W -Wstrict-prototypes -g -c game.c -o game.o

mem.o: mem.c
	gcc -W -Wstrict-prototypes -g -c mem.c -o mem.o

main.o: main.c
	gcc -W -Wstrict-prototypes -g -c main.c -o main.o

buraco: game.o mem.o main.o
	gcc game.o mem.o main.o -o OpenKanastra -lpthread 

clean:
	rm -f *~ *.o OpenKanastra 
