#pragma once

#include "ComponentConcept.h"

namespace Engine
{
    template<ComponentClass C>
    class ComponentRegistry;

    template<ComponentClass C>
    class CompPtrInternal
    {
        friend class ComponentRegistry<C>;
    public:
        C& operator*() const;
        C* operator->();
        C* operator->() const;
    private:
        CompPtrInternal(EntityId entity, C* component);

        EntityId entity;
        C* component;
    };

    template<ComponentClass C>
    class CompPtr
    {
        friend class ComponentRegistry<C>;
    public:
        C& operator*() const;
        C* operator->();
        C* operator->() const;
    private:
        CompPtr(CompPtrInternal<C>* internals);
        CompPtrInternal<C>* internals;
    };



    template<ComponentClass C>
    C& CompPtrInternal<C>::operator*() const
    {
        return *component;
    }

    template<ComponentClass C>
    C * CompPtrInternal<C>::operator->()
    {
        return component;
    }

    template<ComponentClass C>
    C * CompPtrInternal<C>::operator->() const
    {
        return component;
    }


    template<ComponentClass C>
    CompPtrInternal<C>::CompPtrInternal(EntityId entity, C *component):entity{entity},component{component} {}

    template<ComponentClass C>
    C & CompPtr<C>::operator*() const
    {
        return *internals;
    }

    template<ComponentClass C>
    C * CompPtr<C>::operator->()
    {
        return internals->operator->();
    }

    template<ComponentClass C>
    C * CompPtr<C>::operator->() const
    {
        return internals->operator->();
    }

    template<ComponentClass C>
    CompPtr<C>::CompPtr(CompPtrInternal<C> *internals):internals(internals) {}
} // Engine