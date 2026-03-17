#pragma once

#include "engine/core/Window.hpp"

#include <GLFW/glfw3.h>
#include <cstdint>
#include <string>

namespace NexusEngine {
    class GLFWWindow : public Window {
    public:
        GLFWWindow(uint32_t width, uint32_t height, const std::string& title, EventBus& eventBus);
        ~GLFWWindow() override;    

        void Update() override;
        bool ShouldClose() const override;
        uint32_t GetWidth() const override;
        uint32_t GetHeight() const override;
        void* GetNativeWindow() const override;

    private:
        GLFWwindow* m_Window = nullptr;
        uint32_t m_Width;
        uint32_t m_Height;
        EventBus& m_EventBus;

        void SetupCallbacks();
    };
}