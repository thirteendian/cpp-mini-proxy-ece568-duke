all: main

main: main.cpp socket.cpp proxy.cpp request.cpp
	g++ -g  -o main main.cpp socket.cpp proxy.cpp request.cpp

.PHONY:
	clean
clean:
	rm -rf *.o main
