

#ifndef _RESPONSEPARSE_HPP_
#define _RESPONSEPARSE_HPP_
#include "app.hpp"

class ResponseParse {
 private:
  //Response Field
  std::string information;
  int max_age;
  //bool can_cache;
  std::string etag;
  std::string cache_control;
  std::string code;
  time_t date;
  time_t last_modified_date;
  time_t expires_time;
  //
 public:
  //Constructor
  ResponseParse() : information(""), max_age(-1){};
  ResponseParse(std::string information_t) : information(information_t), max_age(-1){};
  //Purse Cohesive
  void parseResponse();
  //Purse
  void parseMaxAge();
  void parseEtag();
  void parseCacheControl();
  void parseCode();
  time_t timeParseHelper();  //for Date and :LastModified
  void parseDate();
  void parseLastModified();
  void parseExpiresTime();

  //Getter
  int getMaxAge() { return max_age; }
  std::string getEtag() { return etag; }
  std::string getCacheControl() { return cache_control; }
  std::string getCode() { return code; }
  time_t getDate() { return date; }
  time_t getLastModifiedDate() { return last_modified_date; }
  time_t getExpiresTime() { return expires_time; }
};

#endif
