all: server c4

server: server.o networking.o
	gcc -o server server.o networking.o

c4: c4.o networking.o
	gcc -o c4 c4.o networking.o

server.o: server.c networking.h
	gcc -c server.c

c4.o: c4.c networking.h
	gcc -c c4.c

networking.o: networking.c networking.h
	gcc -c networking.c

clean:
	rm *.o
	rm *~
