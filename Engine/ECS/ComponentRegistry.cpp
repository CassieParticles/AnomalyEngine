#include "ComponentRegistry.h"

#include "Component.h"

namespace Engine {
    void IComponentRegistry::OnComponentAdded(EntityId entity, Component* component)
    {
        component->entity = entity;
        component->OnComponentAdded();
    }
    void IComponentRegistry::OnComponentRemoved(EntityId entity, Component* component)
    {
        component->OnComponentRemoved();
    }

    void IComponentRegistry::AddInternal(EntityId entity)
    {
        if(HasInternal(entity)){return;}

        uint8_t* component = GetPtrEnt(entity);

        compPtrInternals.insert(std::make_pair(entity,CompPtrInternal(component,entity)));
    }

    void IComponentRegistry::UpdateInternal(EntityId entity)
    {
        if(!HasInternal(entity)){return;}
        CompPtrInternal* internal = GetInternal(entity);
        internal->component = GetPtrEnt(entity);
    }

    void IComponentRegistry::UpdateInternals()
    {
        for(auto pair : compPtrInternals)
        {
            UpdateInternal(pair.first);
        }
    }

    bool IComponentRegistry::HasInternal(EntityId entity) {return compPtrInternals.contains(entity);}
    CompPtrInternal* IComponentRegistry::GetInternal(EntityId entity) {return &compPtrInternals.at(entity);}
} // Engine