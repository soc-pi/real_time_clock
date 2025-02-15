# real_time_clock

## Description

A Linux kernel module implementing an I2C-based Real-Time Clock (RTC) driver for Raspberry Pi 5, with supporting user-space application.

## Project Structure

```
real_time_clock/
├── kernel_module/
│   └── src/
│       ├── real_time_clock.c
│       └── real_time_clock.h
├── application/
│   └── main.c
└── README.md
```

## Features

- I2C-based RTC driver implementation
- BCD time format conversion
- Full RTC register access
- Device tree support
- Supports years 2000-2099
- Standard Linux RTC subsystem integration

## Build

### Kernel Module

```bash
cd real_time_clock/kernel_module
make
```

### Application

```bash
cd real_time_clock/application
gcc -o rtc_test main.c
```

## Usage

### Kernel Module

```bash
# Load the module
sudo insmod kernel_module/src/real_time_clock.ko

# Verify module loading
dmesg | grep rpi5-rtc

# Check RTC device
ls -l /dev/rtc*

# Remove the module
sudo rmmod real_time_clock
```

### Application

```bash
# Run the test application (requires root privileges)
sudo ./rtc_test

# Application will:
# 1. Read current RTC time
# 2. Increment time by 1 minute
# 3. Set new time
# 4. Verify the change
```

### Testing RTC

```bash
# Read current RTC time
sudo hwclock -r

# Set RTC from system time
sudo hwclock -w

# Set system time from RTC
sudo hwclock -s
```

## Technical Details

- I2C Address: 0x68
- Register Map:
  - Seconds: 0x00
  - Minutes: 0x01
  - Hours: 0x02
  - Day: 0x03
  - Date: 0x04
  - Month: 0x05
  - Year: 0x06

## Git Commit Log

### Latest Changes

- Initial RTC driver implementation
- Added I2C communication functions
- Implemented BCD conversion
- Added comprehensive error handling
- Created user-space test application
- Updated documentation with technical details
- Updated documentation with application usage

## License

GPL-2.0
