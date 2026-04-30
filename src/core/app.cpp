#include "core/app.hpp"

#include <iostream>
#include <stdexcept>

void App::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void App::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}

void App::initVulkan() {
    device = new Device(window, true);

    device->createInstance();
    device->setupDebugMessenger();
    device->createSurface(window);
    device->pickPhysicalDevice();
    device->createLogicalDevice();

    swapchain = new Swapchain();
    swapchain->create(
        device->getPhysicalDevice(),
        device->getDevice(),
        device->getSurface(),
        window
    );
    swapchain->createImageViews(device->getDevice());

    pipeline = new Pipeline();
    pipeline->createRenderPass(
        device->getDevice(),
        swapchain->getImageFormat()
    );
    pipeline->createGraphicsPipeline(
        device->getDevice(),
        pipeline->getRenderPass(),
        swapchain->getExtent()
    );
    swapchain->createFramebuffers(device->getDevice(), pipeline->getRenderPass());

    buffers = new Buffers();
    buffers->createVertexBuffer(
        device->getDevice(),
        device->getMemoryProperties(),
        pipeline->getVertices()
    );

    commands = new Commands();
    commands->createPool(
        device->getDevice(),
        device->getQueueFamilies().graphicsFamily.value()
    );
    commands->allocateBuffers(
        device->getDevice(),
        sync->getMaxFramesInFlight()
    );

    sync = new Sync();
    sync->create(device->getDevice());
}

void App::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(device->getDevice());
}

void App::recreateSwapChain() {
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(device->getDevice());

    swapchain->cleanup(device->getDevice());
    pipeline->cleanup(device->getDevice());

    swapchain->create(
        device->getPhysicalDevice(),
        device->getDevice(),
        device->getSurface(),
        window
    );
    swapchain->createImageViews(device->getDevice());

    pipeline->createRenderPass(
        device->getDevice(),
        swapchain->getImageFormat()
    );
    pipeline->createGraphicsPipeline(
        device->getDevice(),
        pipeline->getRenderPass(),
        swapchain->getExtent()
    );
    swapchain->createFramebuffers(device->getDevice(), pipeline->getRenderPass());
}

void App::drawFrame() {
    uint32_t frame = sync->getCurrentFrame();

    sync->wait(device->getDevice(), frame);
    sync->reset(device->getDevice(), frame);

    uint32_t imageIndex = swapchain->acquireNextImage(
        device->getDevice(),
        sync->getImageAvailableSemaphore(frame)
    );

    if (framebufferResized) {
        framebufferResized = false;
        recreateSwapChain();
        return;
    }

    commands->resetBuffer(frame);
    commands->recordBuffer(
        commands->getBuffer(frame),
        imageIndex,
        pipeline->getRenderPass(),
        swapchain->getFramebuffers(),
        swapchain->getExtent(),
        pipeline->getGraphicsPipeline(),
        buffers->getVertexBuffer()
    );

    VkCommandBuffer cmdBuffer = commands->getBuffer(frame);

    VkSemaphore waitSemaphores[] = { sync->getImageAvailableSemaphore(frame) };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    VkSemaphore signalSemaphores[] = { sync->getRenderFinishedSemaphore(frame) };

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(device->getGraphicsQueue(), 1, &submitInfo, sync->getInFlightFence(frame)) != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { swapchain->getSwapchain() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    VkResult result = vkQueuePresentKHR(device->getPresentQueue(), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        recreateSwapChain();
        return;
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to present swap chain image!");
    }

    sync->incrementFrame();
}

void App::cleanup() {
    vkDeviceWaitIdle(device->getDevice());

    swapchain->cleanup(device->getDevice());
    pipeline->cleanup(device->getDevice());
    buffers->cleanup(device->getDevice());
    commands->cleanup(device->getDevice());
    sync->cleanup(device->getDevice());

    delete sync;
    delete commands;
    delete buffers;
    delete pipeline;
    delete swapchain;
    delete device;

    glfwDestroyWindow(window);
    glfwTerminate();
}
