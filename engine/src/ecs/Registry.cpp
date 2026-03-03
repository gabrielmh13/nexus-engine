#include "engine/ecs/Registry.hpp"
#include <cassert>

namespace NexusEngine::ECS {
    Entity Registry::CreateEntity() {
        uint32_t id;

        if(!m_FreeList.empty()){
            id = m_FreeList.back();
            m_FreeList.pop_back();
        } else {
            id = m_Generations.size();
            m_Generations.push_back(0);
        }

        return {id, m_Generations[id]};
    }

    void Registry::DestroyEntity(Entity entity) {
        if(!IsEntityValid(entity))
            return;

        m_Generations[entity.m_Id] += 1;
        m_FreeList.push_back(entity.m_Id);
    }

    bool Registry::IsEntityValid(Entity entity) const {
        uint32_t id = entity.m_Id;
        return id < m_Generations.size() && entity.m_Generation == m_Generations[id];
    }

    uint32_t Registry::GetGeneration(uint32_t entityId) const {
        assert(entityId < m_Generations.size() && "Entity ID out of range");
        return m_Generations[entityId];
    }
}