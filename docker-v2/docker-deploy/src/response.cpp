#include "response.hpp"

#include <string>

//.find point to first char start from 0
//.substr(a,b) start from a(counting from 0) to a+b-1
void Response::parseEtag() {
  size_t etag_start = response.find("Etag:");
  if (etag_start != std::string::npos) {
    etag_start = response.substr(etag_start).find("\"") + 1;

    size_t etag_length = 0;

    etag_length = response.substr(etag_start).find("\"");

    std::string substr = NULL;

    substr = response.substr(etag_start, etag_length);

    etag = substr;
  }
}

void Response::parseMaxAge() {
  size_t maxage_start = response.find("max-age");
  if (maxage_start != std::string::npos) {
    maxage_start = maxage_start + 8;  //after Max-Age=
    std::string substr = NULL;
    size_t iterator = maxage_start;
    while (response.at(iterator) >= '0' && response.at(iterator) <= '9') {
      substr.push_back(response.at(iterator));
      iterator++;
    }
    max_age = substr;
  }
}

void Response::parseCacheControl() {
  size_t cache_info_start = response.find("Cache-Control");
  cache_info_start = cache_info_start + 15;
  if (cache_info_start != std::string::npos) {
    std::string cache_control =
        response.substr(cache_info_start, response.substr(cache_info_start).find("\n"));
    if (cache_control.find("max-age") != std::string::npos) {
      cacheable = true;
      be_public = true;
    }
    if (cache_control.find("private") != std::string::npos) {
      be_public = false;
    }
    if (cache_control.find("no-store") != std::string::npos) {
      isnostore = true;
    }
    if (cache_control.find("no-cache") != std::string::npos) {
      cacheable = false;
    }
    if (cache_control.find("public") != std::string::npos) {
      be_public = true;
    }
    if (cache_control.find("must-revalidate") != std::string::npos) {
      must_revalidate = true;
    }
  }
}

int DaynameConverter(std::string day_name) {
  if (day_name == "Mon") {
    return 1;
  }
  else if (day_name == "Tue") {
    return 2;
  }
  else if (day_name == "Wed") {
    return 3;
  }
  else if (day_name == "Thu") {
    return 4;
  }
  else if (day_name == "Fri") {
    return 5;
  }
  else if (day_name == "Sat") {
    return 6;
  }
  else if (day_name == "Sun") {
    return 7;
  }
  return 0;
}
int MonthConverter(std::string month) {
  if (month == "Jan") {
    return 1;
  }
  else if (month == "Feb") {
    return 2;
  }
  else if (month == "Mar") {
    return 3;
  }
  else if (month == "Apr") {
    return 4;
  }
  else if (month == "May") {
    return 5;
  }
  else if (month == "Jun") {
    return 6;
  }
  else if (month == "Jul") {
    return 7;
  }
  else if (month == "Aug") {
    return 8;
  }
  else if (month == "Sep") {
    return 9;
  }
  else if (month == "Oct") {
    return 10;
  }
  else if (month == "Nov") {
    return 11;
  }
  else if (month == "Dec") {
    return 12;
  }
  return 0;
}

void Response::parseDate() {
  size_t date_start = response.find("Date:");
  int day_time;
  int day;
  int month;
  int year;
  int hour;
  int minute;
  int second;
  std::string substring;
  if (date_start != std::string::npos) {  //parse daytime
    date_start += 6;                      //"Date: Wed, 21 Oct 2015"
    day_time = DaynameConverter(response.substr(date_start, 3));

    //parse day
    substring = response.substr(date_start);  //"Wed, 21 Oct 2015...
    size_t parse_space = substring.find(" ");
    date_start = parse_space + 1;
    day = std::stoi(substring.substr(date_start, 2));
    //parse month
    substring = substring.substr(date_start);  //21 Oct 2015
    parse_space = substring.find(" ");
    date_start = parse_space + 1;
    month = MonthConverter(substring.substr(date_start, 3));
    //parse year
    substring = substring.substr(date_start);  //Oct 2015
    parse_space = substring.find(" ");
    date_start = parse_space + 1;
    year = std::stoi(substring.substr(date_start, 4));
    //parse hour
    substring = substring.substr(date_start);  //2015 07:28:00
    parse_space = substring.find(" ");
    date_start = parse_space + 1;
    hour = std::stoi(substring.substr(date_start, 2));
    //parse minute
    substring = substring.substr(date_start);  //07:28:00
    parse_space = substring.find(":");
    date_start = parse_space + 1;
    minute = std::stoi(response.substr(date_start, 2));
    //parse second
    substring = substring.substr(date_start);  //28:00
    parse_space = substring.find(":");
    date_start = parse_space + 1;
    second = std::stoi(substring.substr(date_start, 2));
    HttpDate parsed_date(day_time, day, month, year, hour, minute, second);

    date = parsed_date;
  }
};

void Response::parseLastModified() {
  size_t date_start = response.find("Last-Modified:");
  int day_time;
  int day;
  int month;
  int year;
  int hour;
  int minute;
  int second;
  std::string substring;
  if (date_start != std::string::npos) {  //parse daytime
    date_start += 6;                      //"Date: Wed, 21 Oct 2015"
    day_time = DaynameConverter(response.substr(date_start, 3));

    //parse day
    substring = response.substr(date_start);  //"Wed, 21 Oct 2015...
    size_t parse_space = substring.find(" ");
    date_start = parse_space + 1;
    day = std::stoi(substring.substr(date_start, 2));
    //parse month
    substring = substring.substr(date_start);  //21 Oct 2015
    parse_space = substring.find(" ");
    date_start = parse_space + 1;
    month = MonthConverter(substring.substr(date_start, 3));
    //parse year
    substring = substring.substr(date_start);  //Oct 2015
    parse_space = substring.find(" ");
    date_start = parse_space + 1;
    year = std::stoi(substring.substr(date_start, 4));
    //parse hour
    substring = substring.substr(date_start);  //2015 07:28:00
    parse_space = substring.find(" ");
    date_start = parse_space + 1;
    hour = std::stoi(substring.substr(date_start, 2));
    //parse minute
    substring = substring.substr(date_start);  //07:28:00
    parse_space = substring.find(":");
    date_start = parse_space + 1;
    minute = std::stoi(response.substr(date_start, 2));
    //parse second
    substring = substring.substr(date_start);  //28:00
    parse_space = substring.find(":");
    date_start = parse_space + 1;
    second = std::stoi(substring.substr(date_start, 2));
    HttpDate parsed_date(day_time, day, month, year, hour, minute, second);
    last_modified = parsed_date;
  }
};

void Response::parseExpires() {
  size_t date_start = response.find("Expires:");
  int day_time;
  int day;
  int month;
  int year;
  int hour;
  int minute;
  int second;
  std::string substring;
  if (date_start != std::string::npos) {
    //parse daytime
    date_start += 6;  //"Date: Wed, 21 Oct 2015"
    day_time = DaynameConverter(response.substr(date_start, 3));

    //parse day
    substring = response.substr(date_start);  //"Wed, 21 Oct 2015...
    size_t parse_space = substring.find(" ");
    date_start = parse_space + 1;
    day = std::stoi(substring.substr(date_start, 2));
    //parse month
    substring = substring.substr(date_start);  //21 Oct 2015
    parse_space = substring.find(" ");
    date_start = parse_space + 1;
    month = MonthConverter(substring.substr(date_start, 3));
    //parse year
    substring = substring.substr(date_start);  //Oct 2015
    parse_space = substring.find(" ");
    date_start = parse_space + 1;
    year = std::stoi(substring.substr(date_start, 4));
    //parse hour
    substring = substring.substr(date_start);  //2015 07:28:00
    parse_space = substring.find(" ");
    date_start = parse_space + 1;
    hour = std::stoi(substring.substr(date_start, 2));
    //parse minute
    substring = substring.substr(date_start);  //07:28:00
    parse_space = substring.find(":");
    date_start = parse_space + 1;
    minute = std::stoi(response.substr(date_start, 2));
    //parse second
    substring = substring.substr(date_start);  //28:00
    parse_space = substring.find(":");
    date_start = parse_space + 1;
    second = std::stoi(substring.substr(date_start, 2));
    HttpDate parsed_date(day_time, day, month, year, hour, minute, second);
    expire_time = parsed_date;
  }
};
