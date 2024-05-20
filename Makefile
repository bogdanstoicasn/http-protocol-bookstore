CC = g++
CFLAGS = -Wall -g -std=c++11

OBJ = client.o connection.o utils.o req.o command.o

all: client

client: $(OBJ)
	$(CC) $(CFLAGS) -o client $(OBJ)

client.o: client.cpp connection.h utils.h req.h command.h
	$(CC) $(CFLAGS) -c client.cpp

connection.o: connection.cpp connection.h
	$(CC) $(CFLAGS) -c connection.cpp

utils.o: utils.cpp utils.h
	$(CC) $(CFLAGS) -c utils.cpp

req.o: req.cpp req.h
	$(CC) $(CFLAGS) -c req.cpp

command.o: command.cpp command.h
	$(CC) $(CFLAGS) -c command.cpp

clean:
	rm -f client $(OBJ)

.PHONY: all clean
