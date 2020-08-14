# If RACK_DIR is not defined when calling the Makefile, default to two directories above
RACK_DIR ?= /opt/rack/Rack-SDK

# FLAGS will be passed to both the C and C++ compiler
FLAGS +=
CFLAGS += -I$(RACK_DIR)/dep/include
CXXFLAGS += -std=c++17

# Careful about linking to shared libraries, since you can't assume much about the user's environment and library search path.
# Static libraries are fine, but they should be added to this plugin's build system.
LDFLAGS +=

SRC_DIRS := src src/widgets src/hilbert src/wavelet src/wind
SRC := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))

# Add .cpp files to the build
SOURCES += $(SRC)

# Add files to the ZIP package when running `make dist`
# The compiled plugin and "plugin.json" are automatically added.
DISTRIBUTABLES += res
DISTRIBUTABLES += $(wildcard LICENSE*)

# Include the Rack plugin Makefile framework
include $(RACK_DIR)/plugin.mk

list:
	$(info Source list is $(SRC) )

