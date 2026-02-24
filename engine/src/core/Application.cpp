#include "engine/core/Application.hpp"
#include "engine/core/Time.hpp"

namespace NexusEngine {
    void Application::Run() {
        OnInit();
    
        m_Running = true;

        while(m_Running){
            Time::Update();
            OnUpdate(Time::DeltaTime());
        }

        OnShutdown();
    }

    void Application::Quit(){
        m_Running = false;
    }
}