CC = g++
CFLAGS = -Wall -g -std=c++11

OBJ = client.o connection.o utils.o req.o cmd.o

# Compile all
all: client

client: $(OBJ)
	$(CC) $(CFLAGS) -o client $(OBJ)

client.o: client.cpp connection.h utils.h req.h cmd.h
	$(CC) $(CFLAGS) -c client.cpp

connection.o: connection.cpp connection.h
	$(CC) $(CFLAGS) -c connection.cpp

utils.o: utils.cpp utils.h
	$(CC) $(CFLAGS) -c utils.cpp

req.o: req.cpp req.h
	$(CC) $(CFLAGS) -c req.cpp

command.o: cmd.cpp cmd.h
	$(CC) $(CFLAGS) -c cmd.cpp

clean:
	rm -f client $(OBJ)

# Archiving
pack:
	zip 325CA_Stoica_Mihai-Bogdan_Tema4PC.zip *.cpp *.h *.hpp Makefile README.md README

# Checker
script: all
	python3 ./checker/checker.py --script ALL ./client

.PHONY: all clean pack script
