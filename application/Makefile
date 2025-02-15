##
# @file Makefile
# @brief Build configuration for RTC test application
# @author Your Name
# @date 2024
#
# This Makefile builds the RTC test application with appropriate warning flags
# and creates the final executable.
##

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

# Project settings
TARGET = rtc_test
SRCS = main.c
DOCS = docs

# Default target
all: $(TARGET)

## @brief Build the RTC test application
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

## @brief Generate documentation using Doxygen
docs:
	mkdir -p $(DOCS)
	doxygen Doxyfile

## @brief Clean built files and documentation
clean:
	rm -f $(TARGET)
	rm -rf $(DOCS)

## @brief Mark targets that don't create files
.PHONY: all clean docs
