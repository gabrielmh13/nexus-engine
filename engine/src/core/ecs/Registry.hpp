#pragma once
#include "engine/core/ecs/Entity.hpp"

namespace NexusEngine::ECS {
    class Registry {
        Entity CreateEntity();
        void DestroyEntity(Entity entity);
        bool IsEntityValid(Entity entity) const;
        uint32_t GetGeneration(uint32_t entityId) const;
    }
}