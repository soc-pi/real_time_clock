#include "rtc_ops.h"
#include "rtc_utils.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  int fd;
  struct rtc_time rtc_tm;

  /* Initialize RTC device */
  fd = rtc_init();
  if (fd < 0) {
    perror("Error opening RTC device");
    exit(EXIT_FAILURE);
  }

  printf("Successfully opened %s\n", RTC_DEVICE);

  /* Read current RTC time */
  if (rtc_read_time(fd, &rtc_tm) < 0) {
    perror("Error reading RTC time");
    rtc_cleanup(fd);
    exit(EXIT_FAILURE);
  }

  printf("Current RTC time: ");
  print_time(&rtc_tm);

  /* Increment and set new time */
  increment_minute(&rtc_tm);
  printf("Setting new time: ");
  print_time(&rtc_tm);

  if (rtc_set_time(fd, &rtc_tm) < 0) {
    perror("Error setting RTC time");
    rtc_cleanup(fd);
    exit(EXIT_FAILURE);
  }

  /* Verify the time was set correctly */
  if (rtc_read_time(fd, &rtc_tm) < 0) {
    perror("Error reading RTC time");
    rtc_cleanup(fd);
    exit(EXIT_FAILURE);
  }

  printf("Verified RTC time: ");
  print_time(&rtc_tm);

  /* Cleanup */
  rtc_cleanup(fd);
  return EXIT_SUCCESS;
}
