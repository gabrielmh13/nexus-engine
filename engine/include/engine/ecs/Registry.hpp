#pragma once

#include "engine/ecs/Entity.hpp"
#include "engine/ecs/ComponentPool.hpp"

#include <cassert>
#include <vector>
#include <typeindex>
#include <unordered_map>
#include <memory>

namespace NexusEngine::ECS {
    class Registry {
    public:
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
        void RemoveComponent(Entity entity){
            assert(IsEntityValid(entity) && "Entity is not valid");
            auto& pool = GetPool<T>();
            pool.Remove(entity.m_Id);
        }

        template<typename T>
        bool HasComponent(Entity entity) {
            assert(IsEntityValid(entity) && "Entity is not valid");
            auto& pool = GetPool<T>();
            return pool.Get(entity.m_Id) != nullptr;
        }
    private:
        std::vector<uint32_t> m_Generations;
        std::vector<uint32_t> m_FreeList;

        std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> m_Pools;

        template<typename T>
        ComponentPool<T>& GetPool(){  
            auto [it, _] = m_Pools.try_emplace(std::type_index(typeid(T)), std::make_unique<ComponentPool<T>>(1024));
            return *static_cast<ComponentPool<T>*>(it->second.get());
        }
    };
}