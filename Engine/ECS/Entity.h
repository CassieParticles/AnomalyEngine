#pragma once
#include "ComponentConcept.h"
#include "Registry.h"

namespace Engine
{
    class Entity
    {
    public:
        Entity();
        Entity(EntityId entity);
        Entity(const Entity& other);

    private:
        EntityId entityId;
    };

}
