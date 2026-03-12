#pragma once

#include "engine/ecs/ComponentPool.hpp"

#include <tuple>
#include <cstddef>
#include <utility>

namespace NexusEngine::ECS {

    class Registry;

    template<typename... Components>
    class View {
    public:
        View(Registry& registry, ComponentPool<Components>&... pools)
            : m_Registry(registry), m_Pools(pools...) 
        {
        }

        template<typename Func>
        void Each(Func&& func){
            std::size_t pivot = FindSmallestPoolIndex();
        }

    private:
        Registry& m_Registry;
        std::tuple<ComponentPool<Components>&...> m_Pools;

        std::size_t FindSmallestPoolIndex() const {
            static_assert(sizeof...(Components) > 0, "View requires at least one component");
            return FindSmallestPoolIndexHelper(std::index_sequence_for<Components...>{});
        }

        template<std::size_t... I>
        std::size_t FindSmallestPoolIndexHelper(std::index_sequence<I...>) const {
            std::size_t sizes[] = { std::get<I>(m_Pools).Size()... };
            std::size_t smallestIndex = 0;
            std::size_t smallestSize = sizes[0];

            for(std::size_t i = 1; i < sizeof...(I); ++i){
                if(sizes[i] < smallestSize){
                    smallestIndex = i;
                    smallestSize = sizes[i];
                }
            }

            return smallestIndex;
        }
    };
}