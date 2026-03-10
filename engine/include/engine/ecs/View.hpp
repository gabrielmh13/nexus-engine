#pragma once

#include "engine/ecs/ComponentPool.hpp"

#include <tuple>

namespace NexusEngine::ECS {

    class Registry;

    template<typename... Components>
    class View {
    public:
        View(Registry& registry, ComponentPool<Components>&... pools)
            : m_Registry(registry), m_Pools(pools...) 
        {
        }

    private:
        Registry& m_Registry;
        std::tuple<ComponentPool<Components>&...> m_Pools;
    };
}