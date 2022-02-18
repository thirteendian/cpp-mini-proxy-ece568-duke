#include <arpa/inet.h>
#include <assert.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

int buildServer(const char* port);
int buildClient(const char* hostname, const char* port);
int acceptConnection(int socket_fd, std::string* ip);
int getPortNum(int socket_fd);