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

    CompPtr<TagComponent> nameA = Registry::GetCompPtr<TagComponent>(entityA);
    CompPtr<TagComponent> nameB = Registry::GetCompPtr<TagComponent>(entityB);
    CompPtr<TagComponent> nameC = Registry::GetCompPtr<TagComponent>(entityC);
    CompPtr<TagComponent> nameD = Registry::GetCompPtr<TagComponent>(entityD);
    CompPtr<TagComponent> nameE = Registry::GetCompPtr<TagComponent>(entityE);

    std::cout<<nameA->name<<std::endl;
    std::cout<<nameB->name<<std::endl;
    std::cout<<nameC->name<<std::endl;
    std::cout<<nameD->name<<std::endl;
    std::cout<<nameE->name<<std::endl;

    entityA.RemoveComponent<TagComponent>();

    std::cout<<nameB->name<<std::endl;
    std::cout<<nameC->name<<std::endl;
    std::cout<<nameD->name<<std::endl;
    std::cout<<nameE->name<<std::endl;
}

void Engine::Application::GameExit()
{
    std::cout<<"Goodbye, World!\n";
}

