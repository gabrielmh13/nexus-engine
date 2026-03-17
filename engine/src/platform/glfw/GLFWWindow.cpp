#include "engine/platform/glfw/GLFWWindow.hpp"
#include "engine/events/WindowEvents.hpp"

namespace NexusEngine {
    GLFWWindow::GLFWWindow(uint32_t width, uint32_t height, const std::string& title, EventBus& eventBus)
        : m_Width(width), m_Height(height), m_EventBus(eventBus) 
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, this);
        SetupCallbacks();
    }

    GLFWWindow::~GLFWWindow() {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void GLFWWindow::Update() {
        glfwPollEvents();
    }

    bool GLFWWindow::ShouldClose() const {
        return glfwWindowShouldClose(m_Window);
    }

    uint32_t GLFWWindow::GetWidth() const { return m_Width; }
    uint32_t GLFWWindow::GetHeight() const { return m_Height; }

    void* GLFWWindow::GetNativeWindow() const { return m_Window; }
    
    void GLFWWindow::SetupCallbacks(){
        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* w) {
            auto* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(w));
            self->m_EventBus.Publish(WindowCloseEvent{});
        });

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* w, int width, int height) {
            auto* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(w));
            self->m_Width = width;
            self->m_Height = height;
            self->m_EventBus.Publish(WindowResizeEvent(width, height));
        });
    }
}