#include <../Engine/EntryPoint/Application.h>
#include <iostream>

#include "ECS/Component.h"

void Engine::Application::Initialize()
{
    Entity player = Registry::CreateEntity("Player");

    CompPtr<TagComponent> playerName = player.GetComponent<TagComponent>();
}

void Engine::Application::GameExit()
{
    std::cout<<"Goodbye, World!\n";
}

