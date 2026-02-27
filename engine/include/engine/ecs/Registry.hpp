#pragma once
#include "engine/ecs/Entity.hpp"

#include<vector>

namespace NexusEngine::ECS {
    class Registry {
    public:
        Entity CreateEntity();
        void DestroyEntity(Entity entity);
        bool IsEntityValid(Entity entity) const;
        uint32_t GetGeneration(uint32_t entityId) const;
    private:
        std::vector<uint32_t> m_Generations;
        std::vector<uint32_t> m_FreeList;
    };
}