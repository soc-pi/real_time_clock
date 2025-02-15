# real_time_clock

## Description

A simple kernel module that provides a real-time clock interface with a user-space application demonstration.

## Project Structure

```
real_time_clock/
├── kernel_module/
│   ├── Makefile
│   └── rtc_module.c
├── user_app/
│   ├── Makefile
│   └── rtc_app.c
└── README.md
```

## Build

### Kernel Module

```bash
cd real_time_clock/kernel_module
make
```

### User Application

```bash
cd real_time_clock/user_app
make
```

## Usage

### Kernel Module

```bash
# Load the module
sudo insmod kernel_module/rtc_module.ko

# Check kernel logs for module info
dmesg | tail

# Remove the module
sudo rmmod rtc_module
```

### User Application

```bash
cd user_app
sudo ./rtc_app
```

## Git Commit Log

### Latest Changes

- Initial project setup
- Added kernel module basic structure
- Implemented RTC functionality in kernel module
- Added user application for RTC interface
- Updated build system with Makefiles
- Documentation improvements

## License

GPL-2.0
