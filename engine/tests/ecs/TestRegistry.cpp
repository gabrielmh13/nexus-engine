#include "engine/ecs/Registry.hpp"

#include <cassert>
#include <print>

struct Position {
    int x, y, z;
};

void TestCreateEntity(){
    NexusEngine::ECS::Registry registry;
    auto entity = registry.CreateEntity();
    assert(entity.m_Id == 0 && entity.m_Generation == 0);
    std::print("[OK] CreateEntity\n");
}

void TestDestroyEntity(){
    NexusEngine::ECS::Registry registry;
    auto entity = registry.CreateEntity();
    registry.DestroyEntity(entity);
    assert(!registry.IsEntityValid(entity));
    std::print("[OK] DestroyEntity\n");
}

void TestEntityIdReuse(){
    NexusEngine::ECS::Registry registry;
    auto entity = registry.CreateEntity();
    registry.DestroyEntity(entity);
    auto entity2 = registry.CreateEntity();
    assert(entity2.m_Id == entity.m_Id);
    assert(entity2.m_Generation == entity.m_Generation + 1);
    std::print("[OK] EntityIdReuse\n");
}

void TestAddComponent(){
    NexusEngine::ECS::Registry registry;
    auto entity = registry.CreateEntity();
    registry.AddComponent<Position>(entity, {0, 0, 0});
    assert(registry.HasComponent<Position>(entity));
    std::print("[OK] AddComponent\n");
}

void TestRemoveComponent(){
    NexusEngine::ECS::Registry registry;
    auto entity = registry.CreateEntity();
    registry.AddComponent<Position>(entity, {0, 0, 0});
    registry.RemoveComponent<Position>(entity);
    assert(!registry.HasComponent<Position>(entity));
    std::print("[OK] RemoveComponent\n");
}

int main() {
    TestCreateEntity();
    TestDestroyEntity();
    TestEntityIdReuse();
    TestAddComponent();
    TestRemoveComponent();
    return 0;
}

