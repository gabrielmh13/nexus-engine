#pragma once
#include "engine/core/Application.hpp"

int main(){
    auto* app = NexusEngine::CreateApplication();
    app->Run();
    delete app;
    return 0;
}