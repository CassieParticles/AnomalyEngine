#pragma once

#include "ComponentConcept.h"

namespace Engine
{
    class CompPtrInternal;

    class IComponentRegistry;

    class Registry;

    class CompPtrInternal
    {
        friend class IComponentRegistry;
    public:
        uint8_t* GetPtr() const {return component;}
    private:
        CompPtrInternal(uint8_t* component, EntityId entity):component{component},entity{entity}{}
        uint8_t* component;
        EntityId entity;
    };

    template<ComponentClass C>
    class CompPtr
    {
        friend class Registry;
    public:
        C* operator->(){return reinterpret_cast<C*>(internal->GetPtr());}

        operator bool()
        {
            if(!internal){return false;}
            return internal->GetPtr();
        }
    private:
        CompPtr(CompPtrInternal* internal):internal{internal}{}
        CompPtrInternal* internal;
    };
} // Engine