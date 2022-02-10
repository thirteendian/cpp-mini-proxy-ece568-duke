#include "app.hpp"

void updateCache(std::unordered_map<std::string, Response> & MyCache,
                 std::string request_information,
                 Response new_response) {
  if (MyCache.find(request_information) != MyCache.end()) {
  }
}
