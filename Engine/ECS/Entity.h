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

        operator EntityId() const{return entityId;}
    private:
        EntityId entityId;
    };

}
