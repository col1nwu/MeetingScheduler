CC = g++
CFLAGES = -g -Wall

all: client serverM serverA

client: client.cpp
	$(CC) $(CFLAGES) $? -o $@

serverM: serverM.cpp
	$(CC) $(CFLAGES) $? -o $@

serverA: serverA.cpp
	$(CC) $(CFLAGES) $? -o $@

clean:
	rm -rf client serverM serverA