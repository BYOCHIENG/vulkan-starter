UNAME_S := $(shell uname -s)

CFLAGS = -std=c++17 -O2

ifeq ($(UNAME_S),Darwin) # Check for MacOS
    CFLAGS += -I/opt/homebrew/include
    LDFLAGS = -L/opt/homebrew/lib -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lglfw -lvulkan -ldl -lpthread
else
    LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
endif

VulkanTest: main.cpp
	g++ $(CFLAGS) -o VulkanTest main.cpp $(LDFLAGS)

.PHONY: test clean

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest
