#include "main.hpp"
#include "socket.hpp"

class Request {
 private:
  std::string request;
  std::string method;
  std::string host;
  std::string path;
  std::string port;
  std::string uri;
  std::string information;

  void parseMethod();
  void parseUri();
  void parseHostPath();
  void parsePort();
  void parseInformation();

 public:
  Request(std::string init_request) : request(init_request) {
    parseMethod();
    parseUri();
    parseHostPath();
    parsePort();
    parseInformation();
  }

  std::string getRequest() { return request; }
  std::string getMethod() { return method; }
  std::string getHost() { return host; }
  std::string getPath() { return path; }
  std::string getPort() { return port; }
  std::string getUri() { return uri; }
  std::string getInforamtion() { return information; }
};

/*
GET http://go.com/ HTTP/1.1
User-Agent: PostmanRuntime/7.29.0
Accept: 
Postman-Token: efaa2e10-c8bd-44f6-8e54-9da0a495a909
Host: go.com
Accept-Encoding: gzip, deflate, br
Connection: keep-alive
*/

// 1. char[] -> vector<char> & string & const char *
// 2. parse (use string)
