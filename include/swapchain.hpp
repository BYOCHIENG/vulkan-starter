#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class Swapchain {
public:
    Swapchain();
    ~Swapchain();

    void create(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface, GLFWwindow* window);
    void createImageViews(VkDevice device);
    void createFramebuffers(VkDevice device, VkRenderPass renderPass);

    void cleanup(VkDevice device);

    uint32_t acquireNextImage(VkDevice device, VkSemaphore semaphore);
    void present(VkQueue presentQueue, VkSwapchainKHR swapchain, VkSemaphore renderFinishedSemaphore, uint32_t imageIndex);

    VkFormat getImageFormat() const { return swapChainImageFormat; }
    VkExtent2D getExtent() const { return swapChainExtent; }
    VkSwapchainKHR getSwapchain() const { return swapChain; }
    const std::vector<VkFramebuffer>& getFramebuffers() const { return swapChainFramebuffers; }

private:
    VkSwapchainKHR swapChain = VK_NULL_HANDLE;
    VkFormat swapChainImageFormat{};
    VkExtent2D swapChainExtent{};

    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkSurfaceKHR surface;
    GLFWwindow* window;

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
};
