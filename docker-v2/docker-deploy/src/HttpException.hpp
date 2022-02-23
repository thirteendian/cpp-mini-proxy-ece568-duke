

#include "main.hpp"

//Exceptions for Socket establishment

class SocketExceptions : public std::exception {
 public:
  std::string message;
  SocketExceptions(std::string whaterror) { message.append(whaterror); }
  const char * what() const throw() { return message.c_str(); }
};
