#include "engine/core/EventBus.hpp"

#include <gtest/gtest.h>
#include <vector>

struct KeyPressEvent : NexusEngine::IEvent {
    int key;
    KeyPressEvent(int key) : key(key) {}
};

struct MouseMoveEvent : NexusEngine::IEvent {
    float x, y;
    MouseMoveEvent(float x, float y) : x(x), y(y) {}
};

TEST(EventBusTest, CallbackCalledAfterProcess) {
    NexusEngine::EventBus bus;
    bool called = false;
    bus.Subscribe<KeyPressEvent>([&](KeyPressEvent&) {
        called = true;
    });
    bus.Publish(KeyPressEvent(87));
    EXPECT_FALSE(called);
    bus.Process();
    EXPECT_TRUE(called);
}

TEST(EventBusTest, CallbackReceivesCorrectData) {
    NexusEngine::EventBus bus;
    int receivedKey = 0;
    bus.Subscribe<KeyPressEvent>([&](KeyPressEvent& e) {
        receivedKey = e.key;
    });
    bus.Publish(KeyPressEvent(87));
    bus.Process();
    EXPECT_EQ(receivedKey, 87);
}

TEST(EventBusTest, MultipleSubscribersCalledForSameEvent) {
    NexusEngine::EventBus bus;
    int count = 0;
    bus.Subscribe<KeyPressEvent>([&](KeyPressEvent&) { count++; });
    bus.Subscribe<KeyPressEvent>([&](KeyPressEvent&) { count++; });
    bus.Publish(KeyPressEvent(87));
    bus.Process();
    EXPECT_EQ(count, 2);
}

TEST(EventBusTest, SubscriberNotCalledForDifferentEvent) {
    NexusEngine::EventBus bus;
    bool called = false;
    bus.Subscribe<KeyPressEvent>([&](KeyPressEvent&) {
        called = true;
    });
    bus.Publish(MouseMoveEvent(1.0f, 2.0f));
    bus.Process();
    EXPECT_FALSE(called);
}

TEST(EventBusTest, PublishWithoutSubscriberDoesNotCrash) {
    NexusEngine::EventBus bus;
    EXPECT_NO_THROW(bus.Publish(KeyPressEvent(87)));
    EXPECT_NO_THROW(bus.Process());
}

TEST(EventBusTest, QueueClearedAfterProcess) {
    NexusEngine::EventBus bus;
    int count = 0;
    bus.Subscribe<KeyPressEvent>([&](KeyPressEvent&) { count++; });
    bus.Publish(KeyPressEvent(87));
    bus.Process();
    bus.Process();
    EXPECT_EQ(count, 1);
}

TEST(EventBusTest, MultipleEventsProcessedInOrder) {
    NexusEngine::EventBus bus;
    std::vector<int> order;
    bus.Subscribe<KeyPressEvent>([&](KeyPressEvent& e) {
        order.push_back(e.key);
    });
    bus.Publish(KeyPressEvent(1));
    bus.Publish(KeyPressEvent(2));
    bus.Publish(KeyPressEvent(3));
    bus.Process();
    ASSERT_EQ(order.size(), 3u);
    EXPECT_EQ(order[0], 1);
    EXPECT_EQ(order[1], 2);
    EXPECT_EQ(order[2], 3);
}

TEST(EventBusTest, MultipleEventTypesProcessedCorrectly) {
    NexusEngine::EventBus bus;
    int keyCount = 0;
    float mouseX = 0;
    bus.Subscribe<KeyPressEvent>([&](KeyPressEvent&) { keyCount++; });
    bus.Subscribe<MouseMoveEvent>([&](MouseMoveEvent& e) { mouseX = e.x; });
    bus.Publish(KeyPressEvent(87));
    bus.Publish(MouseMoveEvent(5.0f, 3.0f));
    bus.Process();
    EXPECT_EQ(keyCount, 1);
    EXPECT_FLOAT_EQ(mouseX, 5.0f);
}