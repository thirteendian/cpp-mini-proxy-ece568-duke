#include "proxy.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>

void Proxy::becomeDaemon() {
  //Step 1 using fork() to create a child process
  pid_t cpid = fork();  //create children process
  //error check for fork()
  if (cpid < 0) {
    exit(EXIT_FAILURE);
  }
  if (cpid > 0) {
    exit(EXIT_SUCCESS);  //have left parent process
  }

  //Step 2 dessociate from controlling tty
  //This will leave it's original process group, avoid
  //sending more signals related to terminal operation to tty

  //setsid() create new ID for child process
  pid_t spid = setsid();
  if (spid < 0) {
    exit(EXIT_FAILURE);
  }

  //Step 3 point stdin/stdout/stderr at /dev/null
  //To dump all output to void that son't be useful, we direct them all to null

  int fd = open("/dev/null", O_RDWR, 0);  //open file with Read and Write permission
  //Now fd represent file descriptor of /dev/null
  if (fd != -1) {
    //dup2(int oldfd, int newfd)
    //point newfd to oldfd
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    if (fd > 2) {
      close(fd);
    }
  }
  //Step 4 Change working directory to "/"
  if (chdir("/") < 0) {
    exit(EXIT_FAILURE);
  }
  //Step 5 Set umask to 0
  //change permission mask to mode&0777
  //The following change to 0 for security reason
  umask(0);

  //Step 6 Use fork() again to make the process not a session leader
  cpid = fork();
  if (cpid < 0) {
    exit(EXIT_FAILURE);
  }
  if (cpid > 0) {
    exit(EXIT_SUCCESS);  //have left parent process
  }
}
/*
std::string Proxy::getRequest() {
  const char * port = "12345";  //12345
  int server_fd;
  server_fd = buildServer(port);

  std::string request;
  struct sockaddr_storage socket_addr;
  socklen_t socket_addr_len = sizeof(socket_addr);
  client_connect_fd =
      accept(server_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
  if (client_connect_fd < 0) {
    std::cout << "Cannot connect with server" << std::endl;
  }

  char buffer[65536] = {0};
  int len = recv(client_connect_fd, buffer, 65536, 0);
  if (len == -1) {
    std::cerr << "Cannot receive" << std::endl;
    exit(EXIT_FAILURE);
  }
  if (len == 0) {
    std::cout << "Disconnect" << std::endl;
  }
  request = buffer;

  return request;
}

bool Proxy::check502(std::string request) {
  if (request.find("\r\n\r\n") == std::string::npos)
    return true;
  else
    return false;
}

bool Proxy::checkMethod(std::string method) {
  if (method == "GET" || method == "POST" || method == "CONNECT")
    return true;
  else
    return false;
}

std::string Proxy::generateSendMessage(std::string method,
                                       std::string path,
                                       std::string information) {
  std::string toSend;
  toSend = method + " " + path + " " + information;
  return toSend;
}
*/
/*
void Proxy::doGet(std::string toSend_str, std::string host_str, std::string port_str) {
//send to server
  const char * toSend = toSend_str.c_str();
  const char * host = host_str.c_str();
  const char * port = port_str.c_str();

  int server_connect_fd = buildClient(host, port);
  if (server_connect_fd < 0) {
    std::cout << "Cannot build client" << std::endl;
    return;
  }

  send(server_connect_fd, toSend, strlen(toSend), 0);

//receive from server
  //std::string response;
  while (1) {
    char buffer[65536] = {0};
    int len = recv(server_connect_fd, buffer, 65536, 0);
    if (len == -1) {
      std::cerr << "Cannot receive" << std::endl;
      exit(EXIT_FAILURE);
    }
    if (len == 0) {
      std::cout << "Disconnect" << std::endl;
      break;
    }

    response_vec.insert(response_vec.end(), buffer, buffer + len);
    std::string res = buffer;
    response_str = response_str + res;

    if (send(client_connect_fd, buffer, len, 0) == -1) {
      std::cerr << "Cannot send" << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  close(server_connect_fd);
}
*/
