#include "rtc_ops.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

int rtc_init(void) { return open(RTC_DEVICE, O_RDWR); }

int rtc_read_time(int fd, struct rtc_time *rtc_tm) {
  return ioctl(fd, RTC_RD_TIME, rtc_tm);
}

int rtc_set_time(int fd, struct rtc_time *rtc_tm) {
  return ioctl(fd, RTC_SET_TIME, rtc_tm);
}

void rtc_cleanup(int fd) { close(fd); }
