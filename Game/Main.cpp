#include <../Engine/EntryPoint/Application.h>
#include <iostream>

#include "Components/SillyTestComponent.h"
#include "ECS/Component.h"

void Engine::Application::Initialize()
{
    Entity playerA = Registry::CreateEntity("PlayerA");
    Entity playerB = Registry::CreateEntity("PlayerB");

    playerA.AddComponent<SillyTestComponent>();
    playerB.AddComponent<GoofyTestComponent>();

    CompPtr<SillyTestComponent> ptr = playerA.GetComponent<SillyTestComponent>();
    CompPtr<GoofyTestComponent> ptr2 = playerB.GetComponent<GoofyTestComponent>();

    ptr->data = 5;
    ptr2->data = 23;

    CompPtr<TestComponent> ptr3 = playerA.GetComponent<TestComponent>();
    CompPtr<TestComponent> ptr4 = playerB.GetComponent<TestComponent>();

    std::cout << ptr3->data << std::endl;
    std::cout << ptr4->data << std::endl;
}

void Engine::Application::GameExit()
{
    std::cout<<"Goodbye, World!\n";
}

