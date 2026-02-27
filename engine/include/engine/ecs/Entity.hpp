#pragma once
#include <cstdint>

namespace NexusEngine::ECS {
    struct Entity {
        uint32_t m_Id;
        uint32_t m_Generation;

        static constexpr uint32_t INVALID = 0xFFFFFFFF;

        constexpr bool operator==(const Entity& other) const noexcept {
            return m_Id == other.m_Id && m_Generation == other.m_Generation;
        }

        constexpr bool operator!=(const Entity& other) const noexcept {
            return !(*this == other);
        }

        static constexpr Entity Invalid() noexcept {
            return { .m_Id = INVALID, .m_Generation = INVALID };
        }

        constexpr bool IsValid() const noexcept {
            return m_Id != INVALID;
        }
    };
}