#include "engine/core/Entrypoint.hpp"
#include "engine/core/Window.hpp"
#include "engine/events/WindowEvents.hpp"

#include <print>
#include <memory>

class NexusGame : public NexusEngine::Application {
    std::unique_ptr<NexusEngine::Window> m_Window;

public:
    void OnInit() override {
        std::print("Application Initialized!\n");

        m_Window = NexusEngine::Window::Create(800, 600, "NexusGame", m_EventBus);

        m_EventBus.Subscribe<NexusEngine::WindowResizeEvent>([](NexusEngine::WindowResizeEvent& event){
            std::print("Window Resized: Width={} | Heigh={}\n", event.width, event.height);
        });

        m_EventBus.Subscribe<NexusEngine::WindowCloseEvent>([this](NexusEngine::WindowCloseEvent&) {
            std::print("Closing...\n");
            Quit();
        });
        
    }
    void OnUpdate(float dt) override {
        m_Window->Update();

        std::print("Delta Time: {} seconds\n", dt);
    }
    void OnShutdown() override {}
};

std::unique_ptr<NexusEngine::Application> NexusEngine::CreateApplication(){
    return std::make_unique<NexusGame>();
}
