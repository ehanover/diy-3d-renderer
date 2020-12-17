# Based on https://spin.atomicobject.com/2016/08/26/makefile-c-projects/

# Types of equal signs: https://stackoverflow.com/a/448939
# Symbols like $@: https://stackoverflow.com/a/3220288

CXX = g++
# CXXFLAGS = -Wall -g
CXXFLAGS = -O2

TARGET_EXEC ?= diy-opengl
BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

DEPS_FLAGS = -MMD -MP # These flags generate .d files which list source file dependencies based on header imports
INCLUDES = -I/usr/include/SDL2 -D_REENTRANT # Find with sdl2-config --cflags (could this be automated?)
LIBS = -lSDL2 # Find with sdl2-config --libs

# Binary
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) $(INCLUDES) $(LIBS) -o $@

# C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPS_FLAGS) $(INCLUDES) -c $< -o $@

-include $(DEPS) # Lets make know about source file dependencies from .d files

.PHONY: install
install:
	install -m 755 build/diy-opengl diy-opengl

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
