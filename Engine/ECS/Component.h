#pragma once
#include "ComponentConcept.h"
#include "Entity.h"
#include "refl.hpp"

namespace Engine
{
    class IComponentRegistry;


    class Component
    {
        friend class IComponentRegistry;
    public:
        Component()=default;
        Entity getEntity() const{return entity;}
    protected:

        virtual void OnComponentAdded(){}
        virtual void OnComponentRemoved(){}

        Entity entity;
    };

    class TagComponent: public Component
    {
    public:
        TagComponent(const std::string& name);
        std::string name;
    private:
    };

    class TestComponent: public Component
    {
    public:
        TestComponent() = default;
        int data;
    private:
    };
}

REFL_AUTO(type(Engine::Component))
REFL_AUTO(type(Engine::TagComponent,bases<Engine::Component>))
REFL_AUTO(type(Engine::TestComponent,bases<Engine::Component>))