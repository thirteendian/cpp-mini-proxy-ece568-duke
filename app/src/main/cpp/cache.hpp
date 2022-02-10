#ifndef _CACHE_HPP_
#define _CACHE_HPP_

#include "app.hpp"

class Cache {
 private:
  std::unordered_map<std::string, Response> MyCache;

 public:
  //constructor
  Cache() : MyCache({}){};
  Cache(std::unordered_map<std::string, Response> & mycache) : MyCache(mycache){};
  //Getter
  std::unordered_map<std::string, Response> getMyCache() { return MyCache; }

  void updateCache(std::unordered_map<std::string, Response> & MyCache,
                   std::string request_information,
                   Response new_response);
};

#endif
