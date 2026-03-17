#include "engine/core/Window.hpp"
#include "engine/core/EventBus.hpp"
#include "engine/events/WindowEvents.hpp"

#include <GLFW/glfw3.h>
#include <cstdint>

namespace NexusEngine {
    class GLFWWindow : public Window {
    public:
        GLFWWindow(uint32_t width, uint32_t height, const std::string& title, EventBus& eventBus)
            : m_Width(width), m_Height(height), m_EventBus(eventBus) 
        {
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
            glfwMakeContextCurrent(m_Window);
            glfwSetWindowUserPointer(m_Window, this);
            SetupCallbacks();
        }

        ~GLFWWindow() override {
            glfwDestroyWindow(m_Window);
            glfwTerminate();
        }

        void Update() override {
            glfwPollEvents();
        }

        bool ShouldClose() const override {
            return glfwWindowShouldClose(m_Window);
        }

        uint32_t GetWidth() const override { return m_Width; }
        uint32_t GetHeight() const override { return m_Height; }

        void* GetNativeWindow() const override { return m_Window; }
    private:
        GLFWwindow* m_Window = nullptr;
        uint32_t m_Width;
        uint32_t m_Height;
        EventBus& m_EventBus;
        
        void SetupCallbacks(){
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
    };

    std::unique_ptr<Window> Window::Create(uint32_t width, uint32_t height, const std::string& title, EventBus& eventBus) {
        return std::make_unique<GLFWWindow>(width, height, title, eventBus);
    }
}