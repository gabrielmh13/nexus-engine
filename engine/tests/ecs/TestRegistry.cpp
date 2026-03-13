#include "engine/ecs/Registry.hpp"

#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

struct Position { float x, y, z; };
struct Velocity { float x, y, z; };
struct Rotation { float x, y, z; };

// ─── Entity Tests ────────────────────────────────────────────────────────────

TEST(RegistryTest, CreateEntity) {
    NexusEngine::ECS::Registry registry;
    auto entity = registry.CreateEntity();
    EXPECT_EQ(entity.m_Id, 0u);
    EXPECT_EQ(entity.m_Generation, 0u);
}

TEST(RegistryTest, DestroyEntity) {
    NexusEngine::ECS::Registry registry;
    auto entity = registry.CreateEntity();
    registry.DestroyEntity(entity);
    EXPECT_FALSE(registry.IsEntityValid(entity));
}

TEST(RegistryTest, EntityIdReuse) {
    NexusEngine::ECS::Registry registry;
    auto entity = registry.CreateEntity();
    registry.DestroyEntity(entity);
    auto entity2 = registry.CreateEntity();
    EXPECT_EQ(entity2.m_Id, entity.m_Id);
    EXPECT_EQ(entity2.m_Generation, entity.m_Generation + 1);
}

// ─── Component Tests ─────────────────────────────────────────────────────────

TEST(RegistryTest, AddComponent) {
    NexusEngine::ECS::Registry registry;
    auto entity = registry.CreateEntity();
    registry.AddComponent<Position>(entity, {0, 0, 0});
    EXPECT_TRUE(registry.HasComponent<Position>(entity));
}

TEST(RegistryTest, RemoveComponent) {
    NexusEngine::ECS::Registry registry;
    auto entity = registry.CreateEntity();
    registry.AddComponent<Position>(entity, {0, 0, 0});
    registry.RemoveComponent<Position>(entity);
    EXPECT_FALSE(registry.HasComponent<Position>(entity));
}

// ─── View Tests ───────────────────────────────────────────────────────────────

class ViewTest : public ::testing::Test {
protected:
    NexusEngine::ECS::Registry registry;
    NexusEngine::ECS::Entity e0, e1, e2;

    void SetUp() override {
        e0 = registry.CreateEntity();
        e1 = registry.CreateEntity();
        e2 = registry.CreateEntity();

        registry.AddComponent<Position>(e0, {1, 2, 3});
        registry.AddComponent<Velocity>(e0, {4, 5, 6});
        registry.AddComponent<Rotation>(e0, {7, 8, 9});

        registry.AddComponent<Position>(e1, {10, 20, 30});
        registry.AddComponent<Velocity>(e1, {40, 50, 60});

        registry.AddComponent<Position>(e2, {100, 200, 300});
        registry.AddComponent<Rotation>(e2, {700, 800, 900});
    }
};

TEST_F(ViewTest, PositionVelocity_ReturnsCorrectEntities) {
    std::vector<uint32_t> visited;
    registry.View<Position, Velocity>().Each([&](uint32_t id, Position& p, Velocity& v) {
        visited.push_back(id);
        if (id == e0.m_Id) {
            EXPECT_FLOAT_EQ(p.x, 1); EXPECT_FLOAT_EQ(p.y, 2); EXPECT_FLOAT_EQ(p.z, 3);
            EXPECT_FLOAT_EQ(v.x, 4); EXPECT_FLOAT_EQ(v.y, 5); EXPECT_FLOAT_EQ(v.z, 6);
        } else if (id == e1.m_Id) {
            EXPECT_FLOAT_EQ(p.x, 10); EXPECT_FLOAT_EQ(p.y, 20); EXPECT_FLOAT_EQ(p.z, 30);
            EXPECT_FLOAT_EQ(v.x, 40); EXPECT_FLOAT_EQ(v.y, 50); EXPECT_FLOAT_EQ(v.z, 60);
        }
    });
    EXPECT_EQ(visited.size(), 2u);
    EXPECT_EQ(std::find(visited.begin(), visited.end(), e2.m_Id), visited.end());
}

TEST_F(ViewTest, PositionRotation_ReturnsCorrectEntities) {
    std::vector<uint32_t> visited;
    registry.View<Position, Rotation>().Each([&](uint32_t id, Position& p, Rotation& r) {
        visited.push_back(id);
        if (id == e0.m_Id) {
            EXPECT_FLOAT_EQ(p.x, 1);   EXPECT_FLOAT_EQ(p.y, 2);   EXPECT_FLOAT_EQ(p.z, 3);
            EXPECT_FLOAT_EQ(r.x, 7);   EXPECT_FLOAT_EQ(r.y, 8);   EXPECT_FLOAT_EQ(r.z, 9);
        } else if (id == e2.m_Id) {
            EXPECT_FLOAT_EQ(p.x, 100); EXPECT_FLOAT_EQ(p.y, 200); EXPECT_FLOAT_EQ(p.z, 300);
            EXPECT_FLOAT_EQ(r.x, 700); EXPECT_FLOAT_EQ(r.y, 800); EXPECT_FLOAT_EQ(r.z, 900);
        }
    });
    EXPECT_EQ(visited.size(), 2u);
    EXPECT_EQ(std::find(visited.begin(), visited.end(), e1.m_Id), visited.end());
}

TEST_F(ViewTest, PositionVelocityRotation_ReturnsOnlyE0) {
    std::vector<uint32_t> visited;
    registry.View<Position, Velocity, Rotation>().Each([&](uint32_t id, Position& p, Velocity& v, Rotation& r) {
        visited.push_back(id);
        EXPECT_EQ(id, e0.m_Id);
        EXPECT_FLOAT_EQ(p.x, 1); EXPECT_FLOAT_EQ(p.y, 2); EXPECT_FLOAT_EQ(p.z, 3);
        EXPECT_FLOAT_EQ(v.x, 4); EXPECT_FLOAT_EQ(v.y, 5); EXPECT_FLOAT_EQ(v.z, 6);
        EXPECT_FLOAT_EQ(r.x, 7); EXPECT_FLOAT_EQ(r.y, 8); EXPECT_FLOAT_EQ(r.z, 9);
    });
    EXPECT_EQ(visited.size(), 1u);
}