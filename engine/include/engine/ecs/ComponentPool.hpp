#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <memory>
#include <algorithm>

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
        static constexpr size_t CHUNK_SIZE = 64;

        ComponentPool(size_t size){
            entities.reserve(size);
            denseComponents.reserve(size);
        }

        T& Add(uint32_t entityId){
            if(GetSparseValue(entityId) != INVALID_INDEX){
                denseComponents[GetSparseValue(entityId)] = T{};
                return denseComponents[GetSparseValue(entityId)];
            }

            uint32_t index = denseComponents.size();
            denseComponents.emplace_back(T{});
            entities.push_back(entityId);
            SetSparseValue(entityId, index);
            return denseComponents[index];
        }

        T& Add(uint32_t entityId, T component){
            if(GetSparseValue(entityId) != INVALID_INDEX){
                denseComponents[GetSparseValue(entityId)] = std::move(component);
                return denseComponents[GetSparseValue(entityId)];
            }

           uint32_t index = denseComponents.size();
           denseComponents.emplace_back(std::move(component));
           entities.push_back(entityId);
           SetSparseValue(entityId, index);
           return denseComponents[index];
        }

        T* Get(uint32_t entityId){
            if(GetSparseValue(entityId) == INVALID_INDEX)
                return nullptr;

            return &denseComponents[GetSparseValue(entityId)];
        }

        void Remove(uint32_t entityId) override {
            if(GetSparseValue(entityId) == INVALID_INDEX)
                return;

            uint32_t index = GetSparseValue(entityId);
            uint32_t lastIndex = denseComponents.size() - 1;

            if(index != lastIndex){
                denseComponents[index] = std::move(denseComponents[lastIndex]);
                entities[index] = entities[lastIndex];
                SetSparseValue(entities[index], index);
            }

            denseComponents.pop_back();
            entities.pop_back();
            SetSparseValue(entityId, INVALID_INDEX);
        }

        std::size_t Size() const {
            return entities.size();
        }

        const std::vector<uint32_t>& GetEntities() const {
            return entities;
        }
    private:
        std::vector<T> denseComponents;
        std::vector<uint32_t> entities;

        std::vector<std::unique_ptr<int32_t[]>> chunks;

        void EnsureCapacity(uint32_t entityId){
            size_t chunkIndex = entityId / CHUNK_SIZE;

            if(chunkIndex >= chunks.size())
                chunks.resize(chunkIndex + 1);

            if(chunks[chunkIndex] == nullptr){
                chunks[chunkIndex] = std::make_unique<int32_t[]>(CHUNK_SIZE);
                std::fill(chunks[chunkIndex].get(), chunks[chunkIndex].get() + CHUNK_SIZE, INVALID_INDEX);
            }
        }

        int32_t GetSparseValue(uint32_t entityId) const noexcept {
            size_t chunkIndex = entityId / CHUNK_SIZE;
            size_t localIndex = entityId % CHUNK_SIZE;

            if(chunkIndex >= chunks.size() || chunks[chunkIndex] == nullptr)
                return INVALID_INDEX;

            return chunks[chunkIndex][localIndex];
        }

        void SetSparseValue(uint32_t entityId, int32_t value){
            EnsureCapacity(entityId);

            size_t chunkIndex = entityId / CHUNK_SIZE;
            size_t localIndex = entityId % CHUNK_SIZE;
            chunks[chunkIndex][localIndex] = value;
        }
    };
}