#include "Registry.h"

#include "Component.h"
#include "Entity.h"

namespace Engine
{

    std::unordered_map<size_t,std::unique_ptr<IComponentRegistry>> Registry::registries;
    std::unordered_map<std::string, ComponentType> Registry::componentTypes;
    EntityId Registry::nextFreeId{};

    Entity Registry::CreateEntity(const std::string& name)
    {
        Entity entity = nextFreeId++;
        entity.AddComponent<TagComponent>(name);
        return entity;
    }

    void Registry::DeleteEntity(EntityId entity)
    {
        for(auto& pair : registries)
        {
            pair.second->DeleteComponent(entity);
        }
    }

    CompPtrInternal* Registry::GetComponentRecurse(EntityId entity, ComponentType* type)
    {
        //If this type has one
        IComponentRegistry* registry = type->registry;
        if(registry!=nullptr)
        {
            if(CompPtrInternal* internal = registry->GetComponent(entity))
            {
                return internal;
            }
        }

        //if type has no children, return nullptr
        if(type->children.size()==0)
        {
            return nullptr;
        }
        //Check each child
        for(auto child : type->children)
        {
            if(CompPtrInternal* internal = GetComponentRecurse(entity,child))
            {
                return internal;
            }
        }
        return nullptr;
    }


    bool Registry::HasComponentRecurse(EntityId entity, ComponentType* type)
    {
        //If this type has one
        if(type->registry->HasComponent(entity))
        {
            return true;
        }
        //if type has no children, return nullptr
        if(type->children.size()==0)
        {
            return false;
        }
        //Check each child
        for(auto child : type->children)
        {
            if(HasComponentRecurse(entity,child))
            {
                return true;
            }
        }
        return false;
    }
} // Engine