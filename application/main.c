/**
 * @file main.c
 * @brief RTC Driver Test Application
 * @author
 * @copyright
 * @date 2024
 *
 * This application demonstrates the usage of the Raspberry Pi 5 RTC driver
 * by performing basic RTC operations:
 * - Opening the RTC device
 * - Reading current time
 * - Setting a new time
 * - Verifying the time was set correctly
 *
 * @note Requires root privileges to access the RTC device
 * @warning Ensure the RTC driver is loaded before running this application
 */

#include <errno.h>
#include <fcntl.h>
#include <linux/rtc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

/** @brief Path to the RTC device file */
#define RTC_DEVICE "/dev/rtc0"

/**
 * @brief Print RTC time in human-readable format
 *
 * Formats and prints the time from an RTC time structure in ISO-like format:
 * YYYY-MM-DD HH:MM:SS
 *
 * @param tm Pointer to RTC time structure to print
 */
void print_time(struct rtc_time *tm) {
  printf("Date/Time: %04d-%02d-%02d %02d:%02d:%02d\n", tm->tm_year + 1900,
         tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
}

/**
 * @brief Main entry point for the RTC test application
 *
 * This function performs the following operations:
 * 1. Opens the RTC device
 * 2. Reads the current time
 * 3. Increments the time by one minute
 * 4. Sets the new time
 * 5. Verifies the time was set correctly
 *
 * @return EXIT_SUCCESS on successful completion, EXIT_FAILURE on error
 *
 * @note The application requires root privileges to access the RTC
 * @warning This will modify the system's RTC time
 */
int main() {
  int fd;                 /**< File descriptor for RTC device */
  int retval;             /**< Return value from system calls */
  struct rtc_time rtc_tm; /**< RTC time structure */

  /* Step 1: Open RTC device */
  fd = open(RTC_DEVICE, O_RDWR);
  if (fd < 0) {
    perror("Error opening RTC device");
    exit(EXIT_FAILURE);
  }

  printf("Successfully opened %s\n", RTC_DEVICE);

  /* Step 2: Read current RTC time */
  retval = ioctl(fd, RTC_RD_TIME, &rtc_tm);
  if (retval < 0) {
    perror("Error reading RTC time");
    close(fd);
    exit(EXIT_FAILURE);
  }

  printf("Current RTC time: ");
  print_time(&rtc_tm);

  /* Step 3: Set new time (current time + 1 minute) */
  rtc_tm.tm_min++;
  if (rtc_tm.tm_min >= 60) {
    rtc_tm.tm_min = 0;
    rtc_tm.tm_hour++;
  }

  printf("Setting new time: ");
  print_time(&rtc_tm);

  /* Step 4: Update RTC with new time */
  retval = ioctl(fd, RTC_SET_TIME, &rtc_tm);
  if (retval < 0) {
    perror("Error setting RTC time");
    close(fd);
    exit(EXIT_FAILURE);
  }

  /* Step 5: Verify the time was set correctly */
  retval = ioctl(fd, RTC_RD_TIME, &rtc_tm);
  if (retval < 0) {
    perror("Error reading RTC time");
    close(fd);
    exit(EXIT_FAILURE);
  }

  printf("Verified RTC time: ");
  print_time(&rtc_tm);

  /* Cleanup */
  close(fd);
  return EXIT_SUCCESS;
}
