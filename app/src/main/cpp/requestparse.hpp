
#ifndef _REQUESTPARSE_HPP_
#define _REQUESTPARSE_HPP_

#include "app.hpp"

class RequestParse {
 private:
  //Request Field
  std::string information;
  std::string method;
  std::string host;
  std::string uri;
  std::string port;
  time_t if_modified_since;
  std::string if_none_match;

 public:
  //Constructor
  RequestParse(std::string information_t) : information(information_t){};

  //Purse Cohesive
  void parseRequest();

  //Purse
  void parseInformation();
  void parseMethod();
  void parseHost();
  void parseUri();
  void parsePort();
  void parseIfModifiedSince();
  void parseIfNoneMatch();

  //Getter
  std::string getInformation;
  std::string getMethod;
  std::string getHost;
  std::string getUri;
  std::string getPort;
  time_t getIfModifiedSince;
  std::string getIfNoneMatch;
};

#endif
