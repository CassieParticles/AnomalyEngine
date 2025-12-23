#pragma once

#include "ComponentConcept.h"

namespace Engine
{
    template<ComponentClass C>
    class CompPtrInternal;

    template<ComponentClass C>
    class ComponentRegistry;

    template<ComponentClass C>
    class CompPtr
    {
        friend class ComponentRegistry<C>;
    public:
        C& operator*(){return *internal;}
        C* operator->(){return internal->operator->();}
        C* operator->() const{return internal->operator->();}
    private:
        CompPtr(CompPtrInternal<C>* internal):internal{internal}{}
        CompPtrInternal<C>* internal;
    };

    template<ComponentClass C>
    class CompPtrInternal
    {
        friend class ComponentRegistry<C>;
    public:
        C& operator*(){return *component;}
        C* operator->(){return component;}
        C* operator->() const{return component;}
    private:
        CompPtrInternal(C* component, EntityId entity):component{component},entity{entity}{}
        C* component;
        EntityId entity;
    };
} // Engine