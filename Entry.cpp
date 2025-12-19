#include <iostream>

#include "Engine/EntryPoint/Application.h"

int main()
{
    Engine::Application application;

    application.Initialize();

    application.GameExit();
}
