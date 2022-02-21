# If chang the excution file name:
TARGET = proxy

CC = g++
CFLAGS = -std=gnu++11 -pedantic -g -Wall
SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

.PHONY: clean depend
clean:
	rm -rf proxy *.o *.h~ *.c~ 
depend:
	makedepend $(SRCS)


main.o: main.hpp
proxy.o: proxy.hpp
request.o: request.hpp
response.o: response.hpp
socket.o: socket.hpp
