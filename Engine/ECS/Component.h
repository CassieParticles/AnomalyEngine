#pragma once
#include "ComponentConcept.h"
#include "Entity.h"
#include "refl.hpp"

namespace Engine
{
    class Component
    {
    public:
        Component();
        Entity getEntity() const{return entity;}
    private:
        Entity entity;
    };


    class TestComponent: public Component
    {
    public:
        TestComponent();
        int data;
    private:
    };
}

REFL_AUTO(type(Engine::Component))
REFL_AUTO(type(Engine::TestComponent,bases<Engine::Component>))