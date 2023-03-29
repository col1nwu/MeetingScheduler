CC = g++
CFLAGS = -g -Wall -std=c++11

SRCS = serverM.cpp serverA.cpp serverB.cpp client.cpp
OBJS = $(SRCS:.cpp=)

all: $(OBJS) $(UTILS)

%: %.cpp
	$(CC) $(CFLAGS) $< utils.cpp -o $@
	chmod u+x $@

clean:
	rm -f $(OBJS)
