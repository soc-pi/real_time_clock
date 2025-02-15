/**
 * @file real_time_clock.c
 * @brief Raspberry Pi 5 RTC Driver Implementation
 * @author Your Name
 * @copyright GPL
 * @date 2024
 *
 * This driver implements support for I2C-based Real-Time Clock chips
 * on the Raspberry Pi 5. It provides basic timekeeping functionality
 * through the Linux RTC subsystem.
 *
 * The driver supports:
 * - Reading current time
 * - Setting time
 * - BCD conversion for RTC registers
 * - Automatic device registration
 *
 * @note This driver requires appropriate device tree configuration
 * @warning This is an example driver and needs testing before production use
 */

#include "real_time_clock.h"
#include <linux/bcd.h>
#include <linux/delay.h> // For msleep
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h> // For device tree lookup

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Raspberry Pi 5 RTC Driver");
MODULE_VERSION("1.0");
MODULE_ALIAS("i2c:rpi5-rtc");

// Helper functions to read and write I2C registers
/**
 * @brief Read a register from the RTC chip
 *
 * @param client Pointer to the I2C client structure
 * @param reg Register address to read from
 * @return Value read from the register, or negative error code on failure
 */
static int rpi5_rtc_read_reg(struct i2c_client *client, u8 reg) {
  struct i2c_msg msg[2];
  u8 buf[1];
  int ret;

  buf[0] = reg;

  msg[0].addr = client->addr;
  msg[0].flags = 0; // Write
  msg[0].len = 1;
  msg[0].buf = buf;

  msg[1].addr = client->addr;
  msg[1].flags = I2C_M_RD; // Read
  msg[1].len = 1;
  msg[1].buf = buf;

  ret = i2c_transfer(client->adapter, msg, 2);
  if (ret != 2) {
    dev_err(&client->dev, "I2C read failed: %d\n", ret);
    return -EIO;
  }

  return buf[0];
}

/**
 * @brief Write a value to a register on the RTC chip
 *
 * @param client Pointer to the I2C client structure
 * @param reg Register address to write to
 * @param value Value to write to the register
 * @return 0 on success, or negative error code on failure
 */
static int rpi5_rtc_write_reg(struct i2c_client *client, u8 reg, u8 value) {
  struct i2c_msg msg;
  u8 buf[2];
  int ret;

  buf[0] = reg;
  buf[1] = value;

  msg.addr = client->addr;
  msg.flags = 0; // Write
  msg.len = 2;
  msg.buf = buf;

  ret = i2c_transfer(client->adapter, &msg, 1);
  if (ret != 1) {
    dev_err(&client->dev, "I2C write failed: %d\n", ret);
    return -EIO;
  }

  return 0;
}

// RTC Driver functions

/**
 * @brief Read the current time from the RTC chip
 *
 * @param dev Pointer to the device structure
 * @param tm Pointer to the rtc_time structure to store the read time
 * @return 0 on success, or negative error code on failure
 */
static int rpi5_rtc_read_time(struct device *dev, struct rtc_time *tm) {
  struct rpi5_rtc_data *data = dev_get_drvdata(dev);
  struct i2c_client *client = data->client;
  int sec, min, hour, day, date, month, year;

  // Read the RTC registers
  sec = rpi5_rtc_read_reg(client, RTC_SECONDS_REG);
  min = rpi5_rtc_read_reg(client, RTC_MINUTES_REG);
  hour = rpi5_rtc_read_reg(client, RTC_HOURS_REG);
  day = rpi5_rtc_read_reg(client, RTC_DAY_REG); // Day of week (not used here)
  date = rpi5_rtc_read_reg(client, RTC_DATE_REG);
  month = rpi5_rtc_read_reg(client, RTC_MONTH_REG);
  year = rpi5_rtc_read_reg(client, RTC_YEAR_REG);

  if (sec < 0 || min < 0 || hour < 0 || date < 0 || month < 0 || year < 0) {
    dev_err(dev, "Error reading RTC registers\n");
    return -EIO;
  }

  // Convert from BCD to decimal
  tm->tm_sec = bcd2bin(sec & 0x7F); // Mask off CH bit (clock halt)
  tm->tm_min = bcd2bin(min);
  tm->tm_hour = bcd2bin(hour & 0x3F); // Mask off 12/24 hour bit if needed
  tm->tm_mday = bcd2bin(date);
  tm->tm_mon = bcd2bin(month & 0x1F) - 1;    // Month is 1-12, tm_mon is 0-11
  tm->tm_year = bcd2bin(year) + 2000 - 1900; // Adjust year

  return 0;
}

/**
 * @brief Set the current time on the RTC chip
 *
 * @param dev Pointer to the device structure
 * @param tm Pointer to the rtc_time structure containing the time to set
 * @return 0 on success, or negative error code on failure
 */
static int rpi5_rtc_set_time(struct device *dev, struct rtc_time *tm) {
  struct rpi5_rtc_data *data = dev_get_drvdata(dev);
  struct i2c_client *client = data->client;
  int ret;

  // Convert from decimal to BCD
  u8 sec = bin2bcd(tm->tm_sec);
  u8 min = bin2bcd(tm->tm_min);
  u8 hour = bin2bcd(tm->tm_hour);
  u8 date = bin2bcd(tm->tm_mday);
  u8 month = bin2bcd(tm->tm_mon + 1); // Month is 0-11, RTC is 1-12
  u8 year =
      bin2bcd(tm->tm_year % 100); // Only store the last two digits of the year

  // Write the RTC registers
  ret = rpi5_rtc_write_reg(client, RTC_SECONDS_REG, sec);
  if (ret < 0)
    return ret;
  ret = rpi5_rtc_write_reg(client, RTC_MINUTES_REG, min);
  if (ret < 0)
    return ret;
  ret = rpi5_rtc_write_reg(client, RTC_HOURS_REG, hour);
  if (ret < 0)
    return ret;
  ret = rpi5_rtc_write_reg(client, RTC_DATE_REG, date);
  if (ret < 0)
    return ret;
  ret = rpi5_rtc_write_reg(client, RTC_MONTH_REG, month);
  if (ret < 0)
    return ret;
  ret = rpi5_rtc_write_reg(client, RTC_YEAR_REG, year);
  if (ret < 0)
    return ret;

  return 0;
}

/**
 * @brief RTC Operations structure
 *
 * Defines the operations that can be performed on the RTC device.
 * Currently supports basic time reading and setting operations.
 */
static const struct rtc_class_ops rpi5_rtc_ops = {
    .read_time = rpi5_rtc_read_time,
    .set_time = rpi5_rtc_set_time,
};

/**
 * @brief I2C Probe function
 *
 * Probes the I2C bus for the RTC device and initializes it.
 *
 * @param client Pointer to the I2C client structure
 * @return 0 on success, or negative error code on failure
 */
static int rpi5_rtc_probe(struct i2c_client *client) {
  struct rpi5_rtc_data *data;
  int err;

  dev_info(&client->dev, "Probing RTC at I2C address 0x%02x\n", client->addr);

  // Allocate memory for our driver data
  data = devm_kzalloc(&client->dev, sizeof(struct rpi5_rtc_data), GFP_KERNEL);
  if (!data) {
    return -ENOMEM;
  }

  data->client = client;
  i2c_set_clientdata(client, data);

  // Create the RTC device
  data->rtc = devm_rtc_allocate_device(&client->dev);
  if (IS_ERR(data->rtc)) {
    return PTR_ERR(data->rtc);
  }

  // Set the RTC operations
  data->rtc->ops = &rpi5_rtc_ops;
  data->rtc->range_min = RTC_TIMESTAMP_BEGIN_2000;
  data->rtc->range_max = RTC_TIMESTAMP_END_2099;

  // Register the RTC device
  err = devm_rtc_register_device(data->rtc);
  if (err) {
    dev_err(&client->dev, "Failed to register RTC device: %d\n", err);
    return err;
  }

  dev_info(&client->dev, "RTC device registered.\n");

  return 0;
}

/**
 * @brief I2C Remove function
 *
 * Removes the RTC device from the I2C bus.
 *
 * @param client Pointer to the I2C client structure
 */
static void rpi5_rtc_remove(struct i2c_client *client) {
  dev_info(&client->dev, "Removing RTC device.\n");

  // The rtc device is automatically unregistered by devm_rtc_register_device.
  // No need to call rtc_unregister_device here.

  // Nothing else to do, memory is managed by devm_kzalloc.
}

/**
 * @brief I2C device ID table
 *
 * Defines the compatible I2C devices for this driver.
 */
static const struct i2c_device_id rpi5_rtc_id[] = {
    {DRIVER_NAME, 0}, {} // Null termination
};
MODULE_DEVICE_TABLE(i2c, rpi5_rtc_id);

/**
 * @brief Device tree compatibility table
 *
 * Defines the compatible device tree entries for this driver.
 */
static const struct of_device_id rpi5_rtc_dt_ids[] = {
    {
        .compatible = "rpi,rpi5-rtc",
    },
    {} // Null termination
};
MODULE_DEVICE_TABLE(of, rpi5_rtc_dt_ids);

/**
 * @brief I2C driver structure
 *
 * Defines the I2C driver operations and properties.
 */
static struct i2c_driver rpi5_rtc_driver = {
    .driver =
        {
            .name = DRIVER_NAME,
            .owner = THIS_MODULE,
            .of_match_table = of_match_ptr(rpi5_rtc_dt_ids),
        },
    .probe = rpi5_rtc_probe,
    .remove = rpi5_rtc_remove,
    .id_table = rpi5_rtc_id,
};

/**
 * @brief Module Initialization function
 *
 * Registers the I2C driver with the I2C subsystem.
 *
 * @return 0 on success, or negative error code on failure
 */
static int __init rpi5_rtc_init(void) {
  return i2c_add_driver(&rpi5_rtc_driver);
}

/**
 * @brief Module Exit function
 *
 * Unregisters the I2C driver from the I2C subsystem.
 */
static void __exit rpi5_rtc_exit(void) { i2c_del_driver(&rpi5_rtc_driver); }

module_init(rpi5_rtc_init);
module_exit(rpi5_rtc_exit);