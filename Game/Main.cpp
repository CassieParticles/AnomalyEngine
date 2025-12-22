#include <../Engine/EntryPoint/Application.h>
#include <iostream>

#include "ECS/Component.h"
#include "ECS/Registry.h"

void Engine::Application::Initialize()
{
    std::cout<<"Hello, World!\n";

    TestComponent* test = Engine::Registry::AddComponent<TestComponent>(0);
    test->data = 23;
}

void Engine::Application::GameExit()
{
    std::cout<<"Goodbye, World!\n";
}

