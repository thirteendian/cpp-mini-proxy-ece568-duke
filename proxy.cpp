#include "proxy.hpp"

std::string Proxy::getRequest(){
  const char* port = "12345"; //12345
  int server_fd;
  server_fd = buildServer(port);

  std::string request;
  struct sockaddr_storage socket_addr;
  socklen_t socket_addr_len = sizeof(socket_addr);
  client_connect_fd = accept(server_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
  if(client_connect_fd < 0){
    std::cout << "Cannot connect with server" << std::endl;
  }
    
  char buffer[65536] = {0};
  int len = recv(client_connect_fd, buffer, 65536, 0);
  if(len == -1){
     std::cerr << "Cannot receive" << std::endl;
     exit(EXIT_FAILURE);
  }
  if(len == 0){
    std::cout << "Disconnect" << std::endl;
  } 
  request = buffer;

  return request;
}

bool Proxy::check502(std::string request){
    if(request.find("\r\n\r\n") == std::string::npos)
        return true;
    else
        return false;
}

bool Proxy::checkMethod(std::string method){
    if(method == "GET" ||  method == "POST" || method == "CONNECT")
        return true;
    else
        return false; 
}

std::string Proxy::generateSendMessage(std::string method, std::string path, std::string information){
  std::string toSend;
  toSend = method + " " + path + " " + information;
  return toSend;
}

void Proxy::doGet(std::string toSend_str, std::string host_str, std::string port_str){
  /******************* Send to Server *******************/
  const char* toSend = toSend_str.c_str();
  const char* host = host_str.c_str();
  const char* port = port_str.c_str();

  int server_connect_fd = buildClient(host, port);
  if(server_connect_fd < 0){
    std::cout << "Cannot build client" << std::endl;
    return;
  }

  send(server_connect_fd, toSend, strlen(toSend), 0);
  
  /***************** Receive from Server *****************/
  //std::string response;
  while(1){
    char buffer[65536] = {0};
    int len = recv(server_connect_fd, buffer, 65536, 0);
    if(len == -1){
      std::cerr << "Cannot receive" << std::endl;
      exit(EXIT_FAILURE);
    }
    if(len == 0){
      std::cout << "Disconnect" << std::endl;
      break;
    }

    response_vec.insert(response_vec.end(), buffer, buffer + len);
    std::string res = buffer;
    response_str = response_str + res; 

    if(send(client_connect_fd, buffer, len, 0) == -1){
      std::cerr << "Cannot send" << std::endl;
      exit(EXIT_FAILURE);
    }   
  }

  close(server_connect_fd);
}

