#include "main.hpp"

class HttpDate {
 private:
  int day_name;
  int day;
  int month;
  int year;
  int hour;
  int minute;
  int second;

 public:
  HttpDate() : day_name(0), day(0), month(0), year(0), hour(0), minute(0), second(0){};
  HttpDate(int day_name, int day, int month, int year, int hour, int minute, int second) :
      day_name(day_name),
      day(day),
      month(month),
      year(year),
      hour(hour),
      minute(minute),
      second(second){};

  int getDayName() { return day_name; }
  int getDay() { return day; }
  int getMonth() { return month; }
  int getYear() { return year; }
  int getHour() { return hour; }
  int getMinite() { return minute; }
  int getSecond() { return second; }
  bool isSmallerThan(HttpDate httpdate);
};
