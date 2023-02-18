CC = g++
CFLAGS = -g -Wall -std=c++11

SRCS = client.cpp serverM.cpp serverA.cpp
OBJS = $(SRCS:.cpp=)

all: $(OBJS) $(UTILS)

%: %.cpp
	$(CC) $(CFLAGS) $< utils.cpp -o $@
	chmod u+x $@

clean:
	rm -f $(OBJS)