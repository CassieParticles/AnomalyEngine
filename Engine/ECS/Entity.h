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

        template<ComponentClass C, typename... Args>
        CompPtr<C> AddComponent(Args... args){return Registry::AddComponent<C>(entityId, args...);}
        template<ComponentClass C>
        CompPtr<C> GetComponentExact(){return Registry::GetComponentExact<C>(entityId);}
        template<ComponentClass C>
        bool HasComponentExact(){return Registry::HasComponentExact<C>(entityId);}
        template<ComponentClass C>
        void RemoveComponent(){Registry::RemoveComponent<C>(entityId);}

        template<ComponentClass C>
        CompPtr<C> GetComponent(){return Registry::GetComponent<C>(entityId);}
        template<ComponentClass C>
        bool HasComponent(){return Registry::HasComponent<C>(entityId);}

        void DeleteEntity(){Registry::DeleteEntity(entityId);}

        operator EntityId() const{return entityId;}
    private:
        EntityId entityId;
    };
}
