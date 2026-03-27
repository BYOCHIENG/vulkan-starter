#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

class VulkanStarter {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    
    GLFWwindow* window;
    const uint32_t WIDTH = 300;
    const uint32_t HEIGHT = 400;

    void initVulkan() {

    }

    void initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Starter", nullptr, nullptr);
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
        } 
    }

    void cleanup() {
        glfwDestroyWindow(window);

        glfwTerminate();
    }

};


int main() {
    VulkanStarter app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
