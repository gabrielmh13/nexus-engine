#include "engine/core/Entrypoint.hpp"

#include <print>
#include <thread>
#include <chrono>
#include <memory>
#include<string>

class TestEvent : public NexusEngine::IEvent {
public:
    TestEvent(std::string msg)
        : m_Msg(std::move(msg)) {}

    std::string m_Msg;
};

class NexusGame : public NexusEngine::Application {
public:
    void OnInit() override {
        std::print("Application Initialized!\n");

        m_EventBus.Subscribe<TestEvent>([](TestEvent& event){
            std::print("EventBus msg: {}", event.m_Msg);
        });

        m_EventBus.Publish(TestEvent("Hello EventBus!\n"));
        
    }
    void OnUpdate(float dt) override {
        std::print("Delta Time: {} seconds\n", dt);

        static float totalTime = 0.0f;
        totalTime += dt;
        if(totalTime >= 1.0f){
            std::print("Total Time: {} seconds\n", totalTime);
            Quit();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    void OnShutdown() override {}
};

std::unique_ptr<NexusEngine::Application> NexusEngine::CreateApplication(){
    return std::make_unique<NexusGame>();
}
