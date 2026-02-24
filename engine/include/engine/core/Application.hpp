#pragma once

namespace NexusEngine {
class Application {
public:
    Application() = default;
    virtual ~Application() = default;

    void Run();
    void Quit();

protected:
    virtual void OnInit() {}
    virtual void OnUpdate(float) {}
    virtual void OnShutdown() {}

private:
    bool m_Running = false;
};
}