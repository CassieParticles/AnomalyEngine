#pragma once

namespace Engine
{
    class Application
    {
    public:
        //Engine initialization/cleanup
        Application();
        ~Application();

        //Game initialization/cleanup, defined by the user
        void Initialize();
        void GameExit();
    private:
    };
} // Engine