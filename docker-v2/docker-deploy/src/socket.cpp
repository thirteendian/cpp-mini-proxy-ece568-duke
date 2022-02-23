#include "socket.hpp"

#include "HttpException.hpp"
void Server::buildServer() {
  memset(&this->host_info, 0, sizeof(this->host_info));  //struct is empt

  //    Note that AF_INET can not return socket addresses for IPv6.
  //    But AF_UNSPEC can return socket addresses for any address family (either IPv4 or IPv6)
  this->host_info.ai_flags = AI_PASSIVE;  // fill in my IP for me, bind socket to host IP
  this->host_info.ai_family = AF_UNSPEC;
  this->host_info.ai_socktype = SOCK_STREAM;  //TCP stream sockets

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
  this->status =
      getaddrinfo(this->hostname, this->port, &this->host_info, &this->host_info_list);
  //host_info_list now points to a linked list of 1 or more struct addr

  //ERROR CHECKING ON GETADDRINFO()
  //Using C The following implementation can be replaced by fpirintf with
  //gai_sterror(status)
  if (this->status != 0) {
    std::string error_message;
    error_message.append("Error: cannot get address info for host  (");
    error_message.append(this->hostname);
    error_message.append(",");
    error_message.append(this->port);
    error_message.append(")\n");
    throw SocketExceptions(error_message);
  }

  ///////////////////////////////////////////////////////////////////////////
  //GET SOCKET FILE DESCRIPTOR
  //int socket(int domain, int type, int protocol);
  //return socket descriptor, -1 on error
  //    what is socket descriptor: Unix file descriptor associated with an open file
  this->socket_fd = socket(this->host_info_list->ai_family,
                           this->host_info_list->ai_socktype,
                           this->host_info_list->ai_protocol);
  //ERROR CHECKING ON SOCKET()
  if (this->socket_fd == -1) {
    std::string error_message;
    error_message.append("Error: cannot create socket for  (");
    error_message.append(this->hostname);
    error_message.append(",");
    error_message.append(this->port);
    error_message.append(")\n");
    throw SocketExceptions(error_message);
  }  //if

  ////////////////////////////////////////////////////////////////////////////
  //BIND SOCKET TO HOST PORT
  //int bind(int sockfd, struct sockaddr *my_addr, int addrlen);
  //if not bind, the kernel choose a local port for us,
  //server we connected will automatically get this information
  //    setsockopt() allow to reuse the port, lose the "Address already in use" error
  int yes = 1;
  this->status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
  this->status = bind(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
  //ERROR CHECKING ON BIND()
  if (status == -1) {
    std::string error_message;
    error_message.append("Error: cannot bind socket  (");
    error_message.append(this->hostname);
    error_message.append(",");
    error_message.append(this->port);
    error_message.append(")\n");
    throw SocketExceptions(error_message);
  }  //if

  ////////////////////////////////////////////////////////////////////////////
  //WAIT FOR CONNECTIONS AND HANDLE IT IN QUEUE
  //int listen(int sockfd, int backlog);
  //backlog is number of connections wait in the queue(normally 20)
  //the connections will wait in queue until accept() them
  this->status = listen(this->socket_fd, 100);
  //ERROR CHECKING ON LISTEN()
  if (this->status == -1) {
    std::string error_message;
    error_message.append("Error: cannot listen on socket  (");
    error_message.append(this->hostname);
    error_message.append(",");
    error_message.append(this->port);
    error_message.append(")\n");
    throw SocketExceptions(error_message);
  }  //if

  //freeaddrinfo(host_info_list);

  //return socket_fd;
}

int Server::acceptConnection(std::string * ip) {
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
      accept(this->socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
  //ERROR CHECKING ON ACCEPT()
  if (client_connect_fd == -1) {
    std::string error_message;
    error_message.append("Error: cannot accept connection on socket");
    throw SocketExceptions(error_message);
  }
  struct sockaddr_in * addr = (struct sockaddr_in *)&socket_addr;
  *ip = inet_ntoa(addr->sin_addr);

  return client_connect_fd;
}

void Client::buildClient() {
  memset(&host_info, 0, sizeof(this->host_info));
  this->host_info.ai_family = AF_UNSPEC;
  this->host_info.ai_socktype = SOCK_STREAM;

  this->status =
      getaddrinfo(this->hostname, this->port, &this->host_info, &this->host_info_list);
  if (this->status != 0) {
    std::string error_message;
    error_message.append("Error: cannot get address info for host  (");
    error_message.append(this->hostname);
    error_message.append(",");
    error_message.append(this->port);
    error_message.append(")\n");
    throw SocketExceptions(error_message);
  }  //if

  this->socket_fd = socket(this->host_info_list->ai_family,
                           this->host_info_list->ai_socktype,
                           this->host_info_list->ai_protocol);
  if (this->socket_fd == -1) {
    std::string error_message;
    error_message.append("Error: cannot create socket  (");
    error_message.append(this->hostname);
    error_message.append(",");
    error_message.append(this->port);
    error_message.append(")\n");
    throw SocketExceptions(error_message);
  }  //if

  this->status = connect(
      this->socket_fd, this->host_info_list->ai_addr, this->host_info_list->ai_addrlen);
  if (this->status == -1) {
    std::string error_message;
    error_message.append("Error: cannot connect to socket  (");
    error_message.append(this->hostname);
    error_message.append(",");
    error_message.append(this->port);
    error_message.append(")\n");
    throw SocketExceptions(error_message);
  }  //if

  //  freeaddrinfo(host_info_list);

  //  return socket_fd;
}

int getPortNum(int socket_fd) {
  struct sockaddr_in sin;
  socklen_t len = sizeof(sin);
  if (getsockname(socket_fd, (struct sockaddr *)&sin, &len) == -1) {
    std::string error_message;
    error_message.append("Error: cannot get socket name");
    throw SocketExceptions(error_message);
  }
  return ntohs(sin.sin_port);
}
