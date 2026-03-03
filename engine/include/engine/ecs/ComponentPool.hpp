#pragma once

#include <unordered_map>
#include <cstdint>

namespace NexusEngine::ECS {
    class IComponentPool {
    public:
        virtual ~IComponentPool() = default;
        virtual void Remove(uint32_t entityId) = 0;
    };

    template<typename T>
    class ComponentPool : public IComponentPool {
    public:
        T& Add(uint32_t entityId){
            auto [it, _] = data.emplace(entityId, T{});
            return it->second;
        }

        T& Add(uint32_t entityId, T component){
            auto [it, _] = data.insert_or_assign(entityId, std::move(component));
            return it->second;
        }

        T* Get(uint32_t entityId){
            auto it = data.find(entityId);
            return it != data.end() ? &it->second : nullptr;
        }

        void Remove(uint32_t entityId) override {
            data.erase(entityId);
        }
    private:
        std::unordered_map<uint32_t, T> data;
    };
}