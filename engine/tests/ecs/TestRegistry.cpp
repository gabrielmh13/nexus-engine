#include "engine/ecs/Registry.hpp"

#include <cassert>
#include <print>
#include <vector>
#include <algorithm>

struct Position {
    float x, y, z;
};

struct Velocity {
    float x, y, z;
};

struct Rotation {
    float x, y, z;
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

void TestView(){
    NexusEngine::ECS::Registry registry;
    auto e0 = registry.CreateEntity();
    auto e1 = registry.CreateEntity();
    auto e2 = registry.CreateEntity();

    registry.AddComponent<Position>(e0, {1, 2, 3});
    registry.AddComponent<Velocity>(e0, {4, 5, 6});
    registry.AddComponent<Rotation>(e0, {7, 8, 9});

    registry.AddComponent<Position>(e1, {10, 20, 30});
    registry.AddComponent<Velocity>(e1, {40, 50, 60});

    registry.AddComponent<Position>(e2, {100, 200, 300});
    registry.AddComponent<Rotation>(e2, {700, 800, 900});

    std::vector<uint32_t> posVelEntities;
    auto view = registry.View<Position, Velocity>();
    view.Each([&](uint32_t id, Position& p, Velocity& v){
        posVelEntities.push_back(id);
        if(id == e0.m_Id){
            assert(p.x == 1 && p.y == 2 && p.z == 3);
            assert(v.x == 4 && v.y == 5 && v.z == 6);
        } else if(id == e1.m_Id){
            assert(p.x == 10 && p.y == 20 && p.z == 30);
            assert(v.x == 40 && v.y == 50 && v.z == 60);
        }
    });
    assert(posVelEntities.size() == 2);
    assert(std::find(posVelEntities.begin(), posVelEntities.end(), e2.m_Id) == posVelEntities.end());
    std::print("[OK] View<Position, Velocity>\n");

    std::vector<uint32_t> posRotEntities;
    auto view2 = registry.View<Position, Rotation>();
    view2.Each([&](uint32_t id, Position& p, Rotation& r){
        posRotEntities.push_back(id);
        if(id == e0.m_Id){
            assert(p.x == 1 && p.y == 2 && p.z == 3);
            assert(r.x == 7 && r.y == 8 && r.z == 9);
        } else if(id == e2.m_Id){
            assert(p.x == 100 && p.y == 200 && p.z == 300);
            assert(r.x == 700 && r.y == 800 && r.z == 900);
        }
    });
    assert(posRotEntities.size() == 2);
    assert(std::find(posRotEntities.begin(), posRotEntities.end(), e1.m_Id) == posRotEntities.end());
    std::print("[OK] View<Position, Rotation>\n");

    std::vector<uint32_t> allEntities;
    auto view3 = registry.View<Position, Velocity, Rotation>();
    view3.Each([&](uint32_t id, Position& p, Velocity& v, Rotation& r){
        allEntities.push_back(id);
        assert(id == e0.m_Id);
        assert(p.x == 1 && p.y == 2 && p.z == 3);
        assert(v.x == 4 && v.y == 5 && v.z == 6);
        assert(r.x == 7 && r.y == 8 && r.z == 9);
    });
    assert(allEntities.size() == 1);
    std::print("[OK] View<Position, Velocity, Rotation>\n");
}

int main() {
    TestCreateEntity();
    TestDestroyEntity();
    TestEntityIdReuse();
    TestAddComponent();
    TestRemoveComponent();
    TestView();
    return 0;
}

