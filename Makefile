# If RACK_DIR is not defined when calling the Makefile, default to two directories above
RACK_DIR ?= ../..

# FLAGS will be passed to both the C and C++ compiler
FLAGS +=
CFLAGS +=
CXXFLAGS +=

# Add .cpp files to the build
SOURCES += $(wildcard CVfunk/src/*.cpp)

# Add files to the ZIP package when running `make dist`
DISTRIBUTABLES += CVfunk/res
DISTRIBUTABLES += $(wildcard CVfunk/LICENSE*)
DISTRIBUTABLES += $(wildcard CVfunk/presets)

# Include the Rack plugin Makefile framework
include $(RACK_DIR)/plugin.mk
