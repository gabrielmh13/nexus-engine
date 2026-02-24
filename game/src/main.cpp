#include "engine/core/Application.hpp"

#include <print>

class NexusGame : public NexusEngine::Application {
public:
    NexusGame() {}
    ~NexusGame() override {}

    void OnInit() override {
        std::print("Application Initialized!\n");
    }
    void OnUpdate(float dt) override {
        std::print("Delta Time: {} seconds\n", dt);

        static float totalTime = 0.0f;
        totalTime += dt;
        if(totalTime >= 1.0f){
            std::print("Total Time: {} seconds\n", totalTime);
            Quit();
        }
    }
    void OnShutdown() override {}
};


int main(){
    NexusGame app;
    app.Run();

    return 0;
}