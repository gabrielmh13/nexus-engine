#pragma once

#include "engine/core/EventBus.hpp"


namespace NexusEngine {
    struct WindowCloseEvent : NexusEngine::IEvent {};

    struct WindowResizeEvent : NexusEngine::IEvent {
        uint32_t width, height;
        WindowResizeEvent(uint32_t w, uint32_t h) : width(w), height(h) {}
    };
}