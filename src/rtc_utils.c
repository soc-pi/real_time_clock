#include "rtc_utils.h"
#include <stdio.h>

void print_time(struct rtc_time *tm) {
  printf("Date/Time: %04d-%02d-%02d %02d:%02d:%02d\n", tm->tm_year + 1900,
         tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
}

void increment_minute(struct rtc_time *tm) {
  tm->tm_min++;
  if (tm->tm_min >= 60) {
    tm->tm_min = 0;
    tm->tm_hour++;
  }
}
