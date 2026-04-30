#pragma once

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "core/device.hpp"
#include "render/swapchain.hpp"
#include "render/pipeline.hpp"
#include "render/buffers.hpp"
#include "render/sync.hpp"
#include "render/commands.hpp"

#include <stdexcept>

class App {
public:
    void run();

private:
    static const uint32_t WIDTH = 400;
    static const uint32_t HEIGHT = 300;

    GLFWwindow* window = nullptr;
    bool framebufferResized = false;

    Device* device;
    Swapchain* swapchain;
    Pipeline* pipeline;
    Buffers* buffers;
    Sync* sync;
    Commands* commands;

    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();
    void recreateSwapChain();
    void drawFrame();

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
};
