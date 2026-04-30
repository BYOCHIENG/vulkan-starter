UNAME_S := $(shell uname -s)

CFLAGS = -g -O0 -std=c++17 -I./include

BUILD_DIR = build
SRC_DIR = src

SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

TARGET = $(BUILD_DIR)/VulkanTest

ifeq ($(UNAME_S),Darwin)
    CFLAGS += -I/opt/homebrew/include
    LDFLAGS = -L/opt/homebrew/lib -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lglfw -lvulkan -ldl -lpthread
else
    LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
endif

SHADER_DIR = shaders
SHADER_SOURCES = $(shell find $(SHADER_DIR) -type f)
SHADER_SPV = $(patsubst $(SHADER_DIR)/%,$(BUILD_DIR)/shaders/%.spv,$(SHADER_SOURCES))

.PHONY: all test clean

all: $(TARGET) $(SHADER_SPV)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	g++ $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	g++ $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(BUILD_DIR)/shaders/%.spv: $(SHADER_DIR)/% | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	glslc $< -o $@

test: all
	./$(TARGET)

debug: all
	gdb $(TARGET)

clean:
	rm -rf $(BUILD_DIR)
