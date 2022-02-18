
#include "HttpDate.hpp"

#include <string>

#include "app.hpp"
bool HttpDate::isSmallerThan(HttpDate httpdate) {
  if (year < httpdate.getYear()) {
    return true;
  }
  else if (month < httpdate.getMonth()) {
    return true;
  }
  else if (day < httpdate.getDay()) {
    return true;
  }
  else if (hour < httpdate.getHour()) {
    return true;
  }
  else if (minute < httpdate.getMinite()) {
    return true;
  }
  else if (second < httpdate.getSecond()) {
    return true;
  }
  return false;
}
