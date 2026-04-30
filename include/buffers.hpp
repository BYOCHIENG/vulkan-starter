#pragma once

#include <vulkan/vulkan.h>

#include "pipeline.hpp"

#include <vector>

class Buffers {
public:
    Buffers();
    ~Buffers();

    void createVertexBuffer(VkDevice device, const VkPhysicalDeviceMemoryProperties& memProperties, const std::vector<Vertex>& vertices);

    void cleanup(VkDevice device);

    VkBuffer getVertexBuffer() const { return vertexBuffer; }

private:
    VkBuffer vertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, const VkPhysicalDeviceMemoryProperties& memProperties);
};
