#include "core/app.hpp"

#include <iostream>
#include <stdexcept>

void App::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

int main() {
    App app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
