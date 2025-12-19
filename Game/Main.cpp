#include <../Engine/EntryPoint/Application.h>
#include <iostream>

void Engine::Application::Initialize()
{
    std::cout<<"Hello, World!\n";
}

void Engine::Application::GameExit()
{
    std::cout<<"Goodbye, World!\n";
}
