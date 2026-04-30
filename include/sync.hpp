#pragma once

#include <vulkan/vulkan.h>

#include <vector>
#include <stdexcept>

class Sync {
public:
    Sync();
    ~Sync();

    void create(VkDevice device);

    void cleanup(VkDevice device);

    void wait(VkDevice device, uint32_t currentFrame);
    void reset(VkDevice device, uint32_t currentFrame);

    uint32_t getCurrentFrame() const { return currentFrame; }
    void incrementFrame();
    uint32_t getMaxFramesInFlight() const { return MAX_FRAMES_IN_FLIGHT; }

    VkSemaphore getImageAvailableSemaphore(uint32_t frame) const { return imageAvailableSemaphores[frame]; }
    VkSemaphore getRenderFinishedSemaphore(uint32_t frame) const { return renderFinishedSemaphores[frame]; }
    VkFence getInFlightFence(uint32_t frame) const { return inFlightFences[frame]; }

private:
    const int MAX_FRAMES_IN_FLIGHT = 2;
    uint32_t currentFrame = 0;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
};
