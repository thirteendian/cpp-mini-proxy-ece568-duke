#ifndef _SOCKET_HPP_
#define _SOCKET_HPP_

#include <netdb.h>
#include <unistd.h>

#include <cstdlib>

#include "main.hpp"

using namespace std;

class Socket {
 public:
  int status;
  int socket_fd;
  /////////////////////////////////////////////////////////////////////////
  //FILL UP THE STRUCT ADDRINFO
  // The following struct is an Linked-List
  // struct addrinfo{
  //        int              ai_flags;     // AI_PASSIVE, AI_CANONNAME, etc.
  //        int              ai_family;    // AF_INET, AF_INET6, AF_UNSPEC
  //        int              ai_socktype;  // SOCK_STREAM, SOCK_DGRAM
  //        int              ai_protocol;  // use 0 for "any"
  //        size_t           ai_addrlen;   // size of ai_addr in bytes
  //        struct sockaddr *ai_addr;      // struct sockaddr_in or _in6
  //        char            *ai_canonname; // full canonical hostname
  //        struct addrinfo *ai_next;      // linked list, next node
  // }
  //
  // struct sockaddr {
  //      unsigned short    sa_family;    // address family, AF_xxx
  //      char              sa_data[14];  // 14 bytes of protocol address
  //  };
  struct addrinfo host_info;
  struct addrinfo * host_info_list;  //will point to the results
  const char * hostname;             //= NULL;      //????????????????
  const char * port;
  //Server Waiting Queue
  //make a socket, bind it and listen on it
  //    return socket_fd    descriptor I
  Socket() : status(0), socket_fd(0), host_info_list(NULL), hostname(NULL), port(NULL) {}
  Socket(const char * port) :
      status(0), socket_fd(0), host_info_list(NULL), hostname(NULL), port(port) {}
  Socket(const char * hostname, const char * port) :
      status(0), socket_fd(0), host_info_list(NULL), hostname(hostname), port(port) {}
  ~Socket() {
    if (socket_fd != 0)
      close(socket_fd);
    if (host_info_list != NULL)
      freeaddrinfo(host_info_list);
  }

  int getPortNum(int socket_fd);
};

//Server Socket
//For proxy establish an server socket listening for client request
class Server : public Socket {
 public:
  Server(const char * port) : Socket(port) { buildServer(); }
  void buildServer();
  //Accept Pending Connections
  //accept the lisening descriptor
  //    return client_connect_fd    descriptor II

  int acceptConnection(std::string * ip);
};

//Client Socket
//For proxy establish an client socket connect to real server
class Client : public Socket {
 public:
  Client(const char * hostname, const char * port) : Socket(hostname, port) {
    buildClient();
  }

  void buildClient();
};
#endif
