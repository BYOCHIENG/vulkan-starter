#pragma once

#include <vulkan/vulkan.h>

#include <vector>
#include <stdexcept>

class Commands {
public:
    Commands();
    ~Commands();

    void createPool(VkDevice device, uint32_t graphicsFamily);
    void allocateBuffers(VkDevice device, uint32_t count);

    void cleanup(VkDevice device);

    void resetBuffer(uint32_t index);
    VkCommandBuffer getBuffer(uint32_t index) const { return commandBuffers[index]; }

    void recordBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, VkRenderPass renderPass, const std::vector<VkFramebuffer>& framebuffers, VkExtent2D swapChainExtent, VkPipeline graphicsPipeline, VkBuffer vertexBuffer);

private:
    VkCommandPool commandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> commandBuffers;
};
