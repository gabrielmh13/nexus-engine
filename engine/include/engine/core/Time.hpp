#pragma once
#include <chrono>

namespace NexusEngine {
class Time {
public:
    Time() = delete;

    static void Update();
    static float DeltaTime();
    static float ElapsedTime();
    static constexpr float FixedDeltaTime() { return 1.0f / 60.0f; }
private:
    static inline bool m_Initialized = false;
    static inline std::chrono::high_resolution_clock::time_point m_StartTime;
    static inline std::chrono::high_resolution_clock::time_point m_LastFrame;
    static inline float m_DeltaTime = 0.0f;
};
}