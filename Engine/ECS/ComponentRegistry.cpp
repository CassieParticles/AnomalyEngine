#include "ComponentRegistry.h"

#include "Component.h"

namespace Engine {
    void IComponentRegistry::OnComponentAdded(EntityId entity, Component *component)
    {
        component->entity = entity;
        component->OnComponentAdded();
    }
    void IComponentRegistry::OnComponentCopied(EntityId entity, Component *component)
    {
        component->entity = entity;
        component->OnComponentAdded();
    }
    void IComponentRegistry::OnComponentRemoved(EntityId entity, Component *component)
    {
        component->OnComponentRemoved();
    }
} // Engine