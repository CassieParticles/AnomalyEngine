#pragma once
#include "ComponentConcept.h"

namespace Engine
{
    class Entity
    {
    public:
        operator EntityId() const {return entityId;}
    private:
        EntityId entityId;
    };
}
