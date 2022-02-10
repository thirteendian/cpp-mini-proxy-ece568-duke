#ifndef _RESPONSE_HPP
#define _RESPONSE_HPP
#include "app.hpp"

class Response {
 private:
  ResponseParse ResponseParse;

 public:
  Response();
  bool check_expires();
  bool check_cacheable();
  bool check_revalidation();
};

#endif
