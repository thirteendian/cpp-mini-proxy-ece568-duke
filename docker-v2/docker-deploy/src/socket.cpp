#include "socket.hpp"

int buildServer(const char * port) {
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
  const char * hostname = NULL;      //????????????????

  memset(&host_info, 0, sizeof(host_info));  //struct is empt

  //    Note that AF_INET can not return socket addresses for IPv6.
  //    But AF_UNSPEC can return socket addresses for any address family (either IPv4 or IPv6)
  host_info.ai_flags = AI_PASSIVE;  // fill in my IP for me, bind socket to host IP
  host_info.ai_family = AF_UNSPEC;
  host_info.ai_socktype = SOCK_STREAM;  //TCP stream sockets

  /////////////////////////////////////////////////////////////////////////
  //DNS LOOKUPS, SERVUCE NAME LOOKUPS, FILLS OUT STRUCT SOCKADDR_IN
  //getaddrinfo()=>addrinfo->sockaddr||sockaddr_in
  //
  //int getaddrinfo(const char *node,     // e.g. "www.example.com" or IP
  //                const char *service,  // e.g. "http"/"telnet"/"ftp" or port number like 80
  //                const struct addrinfo *hints,
  //                struct addrinfo **res)
  //
  // The following is the parallel struct to sockaddr{}
  // For IPV4 sockaddr_in struct can casted to sockaddr struct mentioned above:
  //    struct sockaddr_in{
  //        short int          sin_family;  // Address family, AF_INET
  //        unsigned short int sin_port;    // Port number
  //        struct in_addr     sin_addr;    // Internet address
  //        unsigned char      sin_zero[8]; // Same size as struct sockaddr
  //     }
  status = getaddrinfo(hostname, port, &host_info, &host_info_list);
  //host_info_list now points to a linked list of 1 or more struct addr

  //ERROR CHECKING ON GETADDRINFO()
  //Using C The following implementation can be replaced by fpirintf with
  //gai_sterror(status)
  if (status != 0) {
    cerr << "Error: cannot get address info for host" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  }

  ///////////////////////////////////////////////////////////////////////////
  //GET SOCKET FILE DESCRIPTOR
  //int socket(int domain, int type, int protocol);
  //return socket descriptor, -1 on error
  //    what is socket descriptor: Unix file descriptor associated with an open file
  socket_fd = socket(host_info_list->ai_family,
                     host_info_list->ai_socktype,
                     host_info_list->ai_protocol);
  //ERROR CHECKING ON SOCKET()
  if (socket_fd == -1) {
    cerr << "Error: cannot create socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  }  //if

  ////////////////////////////////////////////////////////////////////////////
  //BIND SOCKET TO HOST PORT
  //int bind(int sockfd, struct sockaddr *my_addr, int addrlen);
  //if not bind, the kernel choose a local port for us,
  //server we connected will automatically get this information
  //    setsockopt() allow to reuse the port, lose the "Address already in use" error
  int yes = 1;
  status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
  status = bind(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
  //ERROR CHECKING ON BIND()
  if (status == -1) {
    cerr << "Error: cannot bind socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  }  //if

  ////////////////////////////////////////////////////////////////////////////
  //WAIT FOR CONNECTIONS AND HANDLE IT IN QUEUE
  //int listen(int sockfd, int backlog);
  //backlog is number of connections wait in the queue(normally 20)
  //the connections will wait in queue until accept() them
  status = listen(socket_fd, 100);
  //ERROR CHECKING ON LISTEN()
  if (status == -1) {
    cerr << "Error: cannot listen on socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  }  //if

  freeaddrinfo(host_info_list);

  return socket_fd;
}

int acceptConnection(int socket_fd, std::string * ip) {
  struct sockaddr_storage socket_addr;
  socklen_t socket_addr_len = sizeof(socket_addr);
  int client_connect_fd;
  //////////////////////////////////////////////////////////////////////////////
  //GET PENDING CONNECTION
  //int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
  //return brand new socket file descriptor(second one)
  //descriptor I is still listening for new connections
  //descirptor II is ready to send() and recv()
  client_connect_fd =
      accept(socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
  //ERROR CHECKING ON ACCEPT()
  if (client_connect_fd == -1) {
    cerr << "Error: cannot accept connection on socket" << endl;
    return -1;
  }
  struct sockaddr_in * addr = (struct sockaddr_in *)&socket_addr;
  *ip = inet_ntoa(addr->sin_addr);

  return client_connect_fd;
}

int buildClient(const char * hostname, const char * port) {
  int status;
  int socket_fd;
  struct addrinfo host_info;
  struct addrinfo * host_info_list;

  memset(&host_info, 0, sizeof(host_info));
  host_info.ai_family = AF_UNSPEC;
  host_info.ai_socktype = SOCK_STREAM;

  status = getaddrinfo(hostname, port, &host_info, &host_info_list);
  if (status != 0) {
    cerr << "Error: cannot get address info for host" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  }  //if

  socket_fd = socket(host_info_list->ai_family,
                     host_info_list->ai_socktype,
                     host_info_list->ai_protocol);
  if (socket_fd == -1) {
    cerr << "Error: cannot create socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  }  //if

  status = connect(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
  if (status == -1) {
    cerr << "Error: cannot connect to socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    return -1;
  }  //if

  freeaddrinfo(host_info_list);

  return socket_fd;
}

int getPortNum(int socket_fd) {
  struct sockaddr_in sin;
  socklen_t len = sizeof(sin);
  if (getsockname(socket_fd, (struct sockaddr *)&sin, &len) == -1) {
    cerr << "Error: cannot get sock name" << endl;
    return -1;
  }
  return ntohs(sin.sin_port);
}
