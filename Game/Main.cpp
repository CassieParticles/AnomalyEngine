#include <../Engine/EntryPoint/Application.h>
#include <iostream>

#include "ECS/Component.h"
#include "ECS/Entity.h"

void Engine::Application::Initialize()
{
    Entity entityA = Registry::CreateEntity("Entity A");
    Entity entityB = Registry::CreateEntity("Entity B");
    Entity entityC = Registry::CreateEntity("Entity C");
    Entity entityD = Registry::CreateEntity("Entity D");
    Entity entityE = Registry::CreateEntity("Entity E");

    CompPtr<TestComponent> test = entityA.AddComponent<TestComponent>();

    test->data = 23;
}

void Engine::Application::GameExit()
{
    std::cout<<"Goodbye, World!\n";
}

