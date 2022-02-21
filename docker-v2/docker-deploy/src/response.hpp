#include "HttpDate.hpp"
#include "main.hpp"
class Response {
 private:
  std::string response;
  std::string etag;
  std::string code;  // 200 OK or 304
  HttpDate date;
  HttpDate last_modified;
  HttpDate expire_time;

  // Cache Control
  /**
   *Note that
   *max-age = public = cacheable
   *we set default as negative
   */

  bool isnostore;
  bool cacheable;
  bool be_public;  //default private is 0
  bool must_revalidate;
  std::string max_age;

  void parseMaxAge();
  void parseEtag();
  void parseCacheControl();
  void parseDate();
  void parseExpires();
  void parseLastModified();

 public:
  Response(std::string init_response) :
      response(init_response),
      etag(""),
      code(""),
      date(),
      last_modified(),
      expire_time(),
      cacheable(false),
      be_public(false),
      max_age(""),
      must_revalidate(false),
      isnostore(false) {
    parseEtag();
    parseCacheControl();
    parseMaxAge();
    parseDate();
    parseExpires();
    parseLastModified();
  }
  bool is_nostore() { return isnostore; }
  bool is_cacheable() { return cacheable; }
  bool is_public() { return be_public; }
  bool must_revalidation() { return must_revalidate; }
  HttpDate getDate() { return date; }
  HttpDate getLastModified() { return last_modified; }
  HttpDate getExpireTime() { return expire_time; }
};
