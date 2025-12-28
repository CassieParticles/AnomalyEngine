#pragma once
#include <ECS/Component.h>


class SillyTestComponent:public Engine::TestComponent
{
public:
    SillyTestComponent()=default;
};

class GoofyTestComponent:public Engine::TestComponent
{
public:
    GoofyTestComponent()=default;
};

REFL_AUTO(type(SillyTestComponent,bases<Engine::TestComponent>))
REFL_AUTO(type(GoofyTestComponent,bases<Engine::TestComponent>))