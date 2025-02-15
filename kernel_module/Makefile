# Makefile for the real_time_clock kernel module


PROJECT_NAME := real_time_clock
obj-m += src/real_time_clock.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
