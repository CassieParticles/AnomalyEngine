#include <../Engine/EntryPoint/Application.h>
#include <iostream>

#include "Components/SillyTestComponent.h"
#include "ECS/Component.h"

class Pack
{
public:
    std::string data;
};

void Engine::Application::Initialize()
{
    Entity entity = Registry::CreateEntity("Hello, my name is Daniel");

    auto ptr = entity.GetComponent<TagComponent>();

    std::cout<<ptr->name<<std::endl;

    for(int i=0;i<1024;++i)
    {
        Entity temp = Registry::CreateEntity("Hello, my name is Paul");
    }

    std::cout<<ptr->name<<std::endl;
}

void Engine::Application::GameExit()
{
    std::cout<<"Goodbye, World!\n";
}

