#pragma once

#include <cstdint>
#include <vector>

namespace NexusEngine::ECS {
    class IComponentPool {
    public:
        virtual ~IComponentPool() = default;
        virtual void Remove(uint32_t entityId) = 0;
    };

    template<typename T>
    class ComponentPool : public IComponentPool {
    public:
        static constexpr int32_t INVALID_INDEX = -1;

        ComponentPool(size_t size){
            sparse.resize(size, INVALID_INDEX);
            entities.reserve(size);
            denseComponents.reserve(size);
        }

        T& Add(uint32_t entityId){
            EnsureCapacity(entityId);

            if(sparse[entityId] != INVALID_INDEX){
                denseComponents[sparse[entityId]] = T{};
                return denseComponents[sparse[entityId]];
            }

            uint32_t index = denseComponents.size();
            denseComponents.emplace_back(T{});
            entities.push_back(entityId);
            sparse[entityId] = index;
            return denseComponents[index];
        }

        T& Add(uint32_t entityId, T component){
            EnsureCapacity(entityId);

            if(sparse[entityId] != INVALID_INDEX){
                denseComponents[sparse[entityId]] = std::move(component);
                return denseComponents[sparse[entityId]];
            }

           uint32_t index = denseComponents.size();
           denseComponents.emplace_back(std::move(component));
           entities.push_back(entityId);
           sparse[entityId] = index;
           return denseComponents[index];
        }

        T* Get(uint32_t entityId){
            if(entityId >= sparse.size() || sparse[entityId] == INVALID_INDEX)
                return nullptr;

            return &denseComponents[sparse[entityId]];
        }

        void Remove(uint32_t entityId) override {
            if(entityId >= sparse.size() || sparse[entityId] == INVALID_INDEX)
                return;

            uint32_t index = sparse[entityId];
            uint32_t lastIndex = denseComponents.size() - 1;

            if(index != lastIndex){
                denseComponents[index] = std::move(denseComponents[lastIndex]);
                entities[index] = entities[lastIndex];
                sparse[entities[index]] = index;
            }

            denseComponents.pop_back();
            entities.pop_back();
            sparse[entityId] = INVALID_INDEX;
        }
    private:
        std::vector<int32_t> sparse;
        std::vector<T> denseComponents;
        std::vector<uint32_t> entities;

        void EnsureCapacity(uint32_t entityId){
            if(entityId >= sparse.size()){
                sparse.resize(entityId + 1, INVALID_INDEX);
            }
        }
    };
}