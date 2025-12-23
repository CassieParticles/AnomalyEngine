#include <../Engine/EntryPoint/Application.h>
#include <iostream>

#include "ECS/Component.h"
#include "ECS/Entity.h"

void Engine::Application::Initialize()
{

}

void Engine::Application::GameExit()
{
    std::cout<<"Goodbye, World!\n";
}

