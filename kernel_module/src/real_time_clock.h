/**
 * @file real_time_clock.h
 * @brief Raspberry Pi 5 RTC Driver Header
 * @author Your Name
 * @copyright GPL
 * @date 2024
 *
 * This header defines the interface for the Raspberry Pi 5 RTC driver.
 * It provides definitions and declarations for interfacing with an I2C-based
 * Real-Time Clock chip, specifically designed for the Raspberry Pi 5.
 */

#ifndef RPI5_RTC_H
#define RPI5_RTC_H

#include <linux/i2c.h>
#include <linux/rtc.h>

/** @brief Driver name used for registration */
#define DRIVER_NAME "rpi5-rtc"

/** @brief Default I2C address for the RTC device */
#define RTC_I2C_ADDRESS 0x68

/**
 * @defgroup rtc_registers RTC Register Addresses
 * @{
 */
#define RTC_SECONDS_REG 0x00 /**< Seconds register */
#define RTC_MINUTES_REG 0x01 /**< Minutes register */
#define RTC_HOURS_REG 0x02   /**< Hours register */
#define RTC_DAY_REG 0x03     /**< Day of week register */
#define RTC_DATE_REG 0x04    /**< Date register */
#define RTC_MONTH_REG 0x05   /**< Month register */
#define RTC_YEAR_REG 0x06    /**< Year register */
#define RTC_CONTROL_REG 0x0E /**< Control register */
#define RTC_STATUS_REG 0x0F  /**< Status register */
/** @} */

/**
 * @struct rpi5_rtc_data
 * @brief RTC driver private data structure
 *
 * This structure holds the private data for the RTC driver instance,
 * including references to the I2C client and RTC device.
 */
struct rpi5_rtc_data {
  struct i2c_client *client; /**< I2C client instance */
  struct rtc_device *rtc;    /**< RTC device instance */
};

/**
 * @brief Read a register from the RTC device
 * @param client I2C client instance
 * @param reg Register address to read
 * @return Register value on success, negative errno on failure
 */
static int rpi5_rtc_read_reg(struct i2c_client *client, u8 reg);

/**
 * @brief Write a value to an RTC register
 * @param client I2C client instance
 * @param reg Register address to write
 * @param value Value to write to the register
 * @return 0 on success, negative errno on failure
 */
static int rpi5_rtc_write_reg(struct i2c_client *client, u8 reg, u8 value);

/**
 * @brief Read the current time from the RTC
 * @param dev Device structure
 * @param tm Time structure to fill
 * @return 0 on success, negative errno on failure
 */
static int rpi5_rtc_read_time(struct device *dev, struct rtc_time *tm);

/**
 * @brief Set the RTC to a specific time
 * @param dev Device structure
 * @param tm Time structure containing the time to set
 * @return 0 on success, negative errno on failure
 */
static int rpi5_rtc_set_time(struct device *dev, struct rtc_time *tm);

/**
 * @brief Probe function called when device is detected
 * @param client I2C client instance
 * @return 0 on success, negative errno on failure
 */
static int rpi5_rtc_probe(struct i2c_client *client);

/**
 * @brief Remove function called when device is removed
 * @param client I2C client instance
 */
static void rpi5_rtc_remove(struct i2c_client *client);

#endif /* RPI5_RTC_H */
