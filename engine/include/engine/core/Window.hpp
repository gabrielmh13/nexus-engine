#pragma once

#include "engine/core/EventBus.hpp"

#include <cstdint>
#include <memory>
#include <string>

namespace NexusEngine {
    class Window {
    public:
        virtual ~Window() = default;

        virtual void Update() = 0;
        virtual bool ShouldClose() const = 0;
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual void* GetNativeWindow() const = 0;

        static std::unique_ptr<Window> Create(
            uint32_t width,
            uint32_t height,
            const std::string& title,
            EventBus& eventBus
        );
    };
}