#ifndef RTC_UTILS_H
#define RTC_UTILS_H

#include <linux/rtc.h>

/**
 * @brief Print RTC time in human-readable format
 * @param tm Pointer to RTC time structure to print
 */
void print_time(struct rtc_time *tm);

/**
 * @brief Increment time by one minute
 * @param tm Pointer to RTC time structure to modify
 */
void increment_minute(struct rtc_time *tm);

#endif /* RTC_UTILS_H */
