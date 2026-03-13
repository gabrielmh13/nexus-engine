#pragma once

#include "engine/ecs/Entity.hpp"
#include "engine/ecs/ComponentPool.hpp"
#include "engine/ecs/View.hpp"

#include <cassert>
#include <vector>
#include <typeindex>
#include <unordered_map>
#include <memory>

namespace NexusEngine::ECS {
    class Registry {
    public:
        static constexpr size_t DEFAULT_POOL_SIZE = 4096;

        Entity CreateEntity();
        void DestroyEntity(Entity entity);
        bool IsEntityValid(Entity entity) const;
        uint32_t GetGeneration(uint32_t entityId) const;

        template<typename T>
        T& AddComponent(Entity entity){
            assert(IsEntityValid(entity) && "Entity is not valid");
            auto& pool = GetPool<T>();
            return pool.Add(entity.m_Id);
        }

        template<typename T>
        T& AddComponent(Entity entity, T component){
            assert(IsEntityValid(entity) && "Entity is not valid");
            auto& pool = GetPool<T>();
            return pool.Add(entity.m_Id, std::move(component));
        }

        template<typename T>
        T* GetComponent(Entity entity){
            if(!IsEntityValid(entity))
                return nullptr;
            auto& pool = GetPool<T>();
            return pool.Get(entity.m_Id);
        }

        template<typename T>
        void RemoveComponent(Entity entity){
            if(!IsEntityValid(entity))
                return;
            auto& pool = GetPool<T>();
            pool.Remove(entity.m_Id);
        }

        template<typename T>
        bool HasComponent(Entity entity) {
            if(!IsEntityValid(entity))
                return false;
            auto& pool = GetPool<T>();
            return pool.Get(entity.m_Id) != nullptr;
        }

        template<typename T>
        void RegisterComponent(size_t size = DEFAULT_POOL_SIZE){
            m_Pools.try_emplace(std::type_index(typeid(T)), std::make_unique<ComponentPool<T>>(size));
        }

        template<typename... Components>
        auto View(){
            return NexusEngine::ECS::View<Components...>(GetPool<Components>()...);
        }

    private:
        std::vector<uint32_t> m_Generations;
        std::vector<uint32_t> m_FreeList;

        std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> m_Pools;

        template<typename T>
        ComponentPool<T>& GetPool(){  
            auto [it, _] = m_Pools.try_emplace(std::type_index(typeid(T)), std::make_unique<ComponentPool<T>>(DEFAULT_POOL_SIZE));
            return *static_cast<ComponentPool<T>*>(it->second.get());
        }
    };
}