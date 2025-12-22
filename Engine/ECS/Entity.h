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
        C* AddComponent(Args... args);

        template<ComponentClass C>
        void RemoveComponent();

        template<ComponentClass C>
        C* GetComponent();

        template<ComponentClass C>
        bool HasComponent();

        operator EntityId() const {return entityId;}
    private:
        EntityId entityId;
    };

    template<ComponentClass C, typename ... Args>
    C * Entity::AddComponent(Args... args)
    {
        return Registry::AddComponent<C>(entityId, args...);
    }

    template<ComponentClass C>
    void Entity::RemoveComponent()
    {
        Registry::RemoveComponent<C>(entityId);
    }

    template<ComponentClass C>
    C * Entity::GetComponent()
    {
        return Registry::GetComponent<C>(entityId);
    }

    template<ComponentClass C>
    bool Entity::HasComponent()
    {
        return Registry::HasComponent<C>(entityId);
    }
}
