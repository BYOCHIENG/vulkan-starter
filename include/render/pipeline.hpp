#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <array>
#include <stdexcept>

struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
};

class Pipeline {
public:
    Pipeline();
    ~Pipeline();

    void createRenderPass(VkDevice device, VkFormat swapChainImageFormat);
    void createGraphicsPipeline(VkDevice device, VkRenderPass renderPass, VkExtent2D swapChainExtent);
    void recreateGraphicsPipeline(VkDevice device, VkPipelineLayout pipelineLayout, VkExtent2D swapChainExtent);

    void cleanup(VkDevice device);

    VkRenderPass getRenderPass() const { return renderPass; }
    VkPipeline getGraphicsPipeline() const { return graphicsPipeline; }
    VkPipelineLayout getPipelineLayout() const { return pipelineLayout; }
    const std::vector<Vertex>& getVertices() const { return vertices; }

private:
    VkRenderPass renderPass = VK_NULL_HANDLE;
    VkPipeline graphicsPipeline = VK_NULL_HANDLE;
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;

    std::vector<Vertex> vertices = {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

    static std::vector<char> readFile(const std::string& filename);
    VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);
};
