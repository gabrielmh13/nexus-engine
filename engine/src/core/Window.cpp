#include "engine/core/Window.hpp"
#include "engine/platform/glfw/GLFWWindow.hpp"

namespace NexusEngine {
    std::unique_ptr<Window> Window::Create(uint32_t width, uint32_t height, const std::string& title, EventBus& eventBus){
        return std::make_unique<GLFWWindow>(width, height, title, eventBus);
    }
}