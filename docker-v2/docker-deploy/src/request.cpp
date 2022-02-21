#include "request.hpp"

void Request::parseMethod(){
  size_t method_pos = request.find(' ');
  method = request.substr(0, method_pos);
}

void Request::parseUri(){
  size_t uri_start = request.find(' ');
  uri_start++;
  size_t uri_end = request.find(' ', uri_start);
  size_t uri_len = uri_end - uri_start;
  uri = request.substr(uri_start, uri_len);
}

void Request::parseHostPath(){
  size_t host_start = uri.find("//");
  if(host_start == std::string::npos)
    host_start = 0;
  else
    host_start += 2;

  size_t host_end = uri.find_first_of("/", host_start);
  host = uri.substr(host_start, host_end - host_start);

  path = uri.substr(host_end);
}

void Request::parsePort(){
  size_t host_start = request.find("Host: ");
  size_t host_end = request.find_first_of("\r\n", host_start);
  host_start += 6;
  std::string line = request.substr(host_start, host_end - host_start);
  size_t port_start = line.find_first_of(":\r", host_start);
  if(port_start == std::string::npos)
    port = "80";
  else
    port = request.substr(port_start, host_end - port_start - 1);
}

void Request::parseInformation(){
  size_t pos = request.find("HTTP/1.1");
  information = request.substr(pos);
}