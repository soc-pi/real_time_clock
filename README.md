# real_time_clock

## Description
A simple kernel module with a user-space application.

## Project Structure
```
real_time_clock/
├── kernel_module/
│   ├── src/
│   │   └── real_time_clock.c
│   ├── include/
│   │   └── real_time_clock.h
│   └── Makefile
└── application/
    ├── main.c
    └── CMakeLists.txt
```

## Build

### Kernel Module
```bash
cd real_time_clock/kernel_module
make
```

### Application
```bash
cd real_time_clock/application
cmake .
make
```

## Usage

### Kernel Module
```bash
sudo insmod real_time_clock/kernel_module/real_time_clock.ko
# Do something with the module
sudo rmmod real_time_clock
```

### Application
```bash
cd real_time_clock/application
./test_app
```

## License
GPL
