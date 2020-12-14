# Based on https://spin.atomicobject.com/2016/08/26/makefile-c-projects/

# Types of equal signs: https://stackoverflow.com/a/448939
# Symbols like $@: https://stackoverflow.com/a/3220288

CXX = g++
CXXFLAGS = -Wall -g

TARGET_EXEC ?= a.out
BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

DEPS_FLAGS = -MMD -MP # These flags generate .d files which list source file dependencies based on header imports
INCLUDES = -I/usr/include/SFML
LIBS = -L/usr/lib/x86_64-linux-gnu/ -lsfml-graphics -lsfml-window -lsfml-system

# Binary
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) $(INCLUDES) $(LIBS) -o $@

# C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPS_FLAGS) $(INCLUDES) -c $< -o $@

-include $(DEPS) # Lets make know about source file dependencies from .d files

.PHONY: clean

clean:
	rm -r $(BUILD_DIR)
