#pragma once

#include "engine/ecs/ComponentPool.hpp"

#include <tuple>
#include <cstddef>
#include <utility>

namespace NexusEngine::ECS {

    template<typename... Components>
    class View {
    public:
        View(ComponentPool<Components>&... pools)
            : m_Pools(pools...) 
        {
        }

        template<typename Func>
        void Each(Func&& func){
            std::size_t pivot = FindSmallestPoolIndex();
            Dispatch(std::forward<Func>(func), pivot, std::index_sequence_for<Components...>{});
        }

    private:
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

        template<typename Func, std::size_t... I>
        void Dispatch(Func&& func, std::size_t pivotIndex, std::index_sequence<I...>){
            ((I == pivotIndex && (IteratePool<I>(std::forward<Func>(func), std::index_sequence_for<Components...>{}), true)), ...);
        }

        template<std::size_t PivotIndex, typename Func, std::size_t... I>
        void IteratePool(Func&& func, std::index_sequence<I...>){
            auto& pool = std::get<PivotIndex>(m_Pools);

            for(uint32_t entityId : pool.GetEntities()){
                auto componentPtrs  = std::make_tuple(std::get<I>(m_Pools).Get(entityId)...);

                bool hasAllComponents = std::apply([](auto*... cp){
                    return (... && (cp != nullptr));
                }, componentPtrs);

                if(!hasAllComponents) continue;

                std::apply([&](auto*... cp){
                    func(entityId, *cp...);
                }, componentPtrs);
            }
        }
    };
}