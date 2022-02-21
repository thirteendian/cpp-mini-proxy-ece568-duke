#include <arpa/inet.h>
#include <assert.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;

//Server Waiting Queue
//make a socket, bind it and listen on it
//    return socket_fd    descriptor I
int buildServer(const char * port);
//Accept Pending Connections
//accept the lisening descriptor
//    return client_connect_fd    descriptor II
int acceptConnection(int socket_fd, std::string * ip);

int buildClient(const char * hostname, const char * port);

int getPortNum(int socket_fd);
