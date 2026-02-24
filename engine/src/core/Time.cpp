#include "engine/core/Time.hpp"

namespace NexusEngine {
    void Time::Update(){
        auto now = std::chrono::high_resolution_clock::now();

        if(!m_Initialized){
            m_StartTime = now;
            m_LastFrame = now;
            m_Initialized = true;
            return;
        }

        m_DeltaTime = std::chrono::duration<float>(now - m_LastFrame).count();
        m_LastFrame = now;
    }

    float Time::DeltaTime(){
        return m_DeltaTime;
    }

    float Time::ElapsedTime(){
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<float>(now - m_StartTime).count();
    }
}