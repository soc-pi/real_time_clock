#ifndef RTC_OPS_H
#define RTC_OPS_H

#include <linux/rtc.h>

/** @brief Path to the RTC device file */
#define RTC_DEVICE "/dev/rtc0"

/**
 * @brief Initialize RTC device
 * @return File descriptor on success, -1 on failure
 */
int rtc_init(void);

/**
 * @brief Read current RTC time
 * @param fd RTC file descriptor
 * @param rtc_tm Pointer to store RTC time
 * @return 0 on success, -1 on failure
 */
int rtc_read_time(int fd, struct rtc_time *rtc_tm);

/**
 * @brief Set RTC time
 * @param fd RTC file descriptor
 * @param rtc_tm Pointer to new RTC time
 * @return 0 on success, -1 on failure
 */
int rtc_set_time(int fd, struct rtc_time *rtc_tm);

/**
 * @brief Close RTC device
 * @param fd RTC file descriptor
 */
void rtc_cleanup(int fd);

#endif /* RTC_OPS_H */
