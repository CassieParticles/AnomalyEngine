#include <../Engine/EntryPoint/Application.h>
#include <iostream>

#include "ECS/Component.h"
#include "ECS/ComponentRegistry.h"

void Engine::Application::Initialize()
{
    std::cout<<"Hello, World!\n";


    ComponentRegistry<TestComponent> registry;

    TestComponent* test = registry.AddComponent(0);
    TestComponent* test2 = registry.AddComponent(1);
    TestComponent* test3 = registry.AddComponent(2);
    TestComponent* test4 = registry.AddComponent(3);
    TestComponent* test5 = registry.AddComponent(4);
    if (registry.HasComponent(0))
    {
        std::cout<<"Has, component!\n";
    }
    test->data = 23;

    std::cout<<dynamic_cast<TestComponent*>(registry.GetComponent(0))->data<<std::endl;

    registry.RemoveComponent(0);

    if (!registry.HasComponent(0))
    {
        std::cout<<"Has no, component!\n";
    }
}

void Engine::Application::GameExit()
{
    std::cout<<"Goodbye, World!\n";
}

