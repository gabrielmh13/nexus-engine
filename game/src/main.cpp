#include "engine/core/Application.hpp"
#include "engine/ecs/Registry.hpp"

#include <print>
#include <thread>
#include <chrono>

class NexusGame : public NexusEngine::Application {
public:
    NexusGame() {}
    ~NexusGame() override {}

    void OnInit() override {
        std::print("Application Initialized!\n");

        NexusEngine::ECS::Registry registry;
        auto entity = registry.CreateEntity();
        std::print("Entity created: {} {} \n", entity.m_Id, entity.m_Generation);

        registry.DestroyEntity(entity);
        std::print("Entity destroyed: {} {} \n", entity.m_Id, entity.m_Generation);
        auto entity2 = registry.CreateEntity();
        std::print("Entity created: {} {} \n", entity2.m_Id, entity2.m_Generation);

        auto entity3 = registry.CreateEntity();
        std::print("Entity created: {} {} \n", entity3.m_Id, entity3.m_Generation);
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


int main(){
    NexusGame app;
    app.Run();

    return 0;
}