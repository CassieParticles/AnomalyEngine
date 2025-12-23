#pragma once
#include <memory>
#include <unordered_map>

#include "ComponentConcept.h"
#include "ComponentRegistry.h"

namespace Engine
{
    class Registry
    {
    public:
        template<ComponentClass C,typename... Args>
        static CompPtr<C> AddComponent(EntityId entity,Args... args);
        template<ComponentClass C>
        static CompPtr<C> GetComponent(EntityId entity);
        template<ComponentClass C>
        static bool HasComponent(EntityId entity);
        template<ComponentClass C>
        static void RemoveComponent(EntityId entity);
    protected:
        //Component registries
        template<ComponentClass C>
        static ComponentRegistry<C>* AddRegistry();
        template<ComponentClass C>
        static ComponentRegistry<C>* GetRegistry();
        template<ComponentClass C>
        static bool HasRegistry();

        static std::unordered_map<size_t, std::unique_ptr<IComponentRegistry>> registries;
    };

    template<ComponentClass C, typename ... Args>
    CompPtr<C> Registry::AddComponent(EntityId entity, Args... args)
    {
        ComponentRegistry<C>* registry = AddRegistry<C>();
        return registry->AddComponent(entity, args...);
    }

    template<ComponentClass C>
    CompPtr<C> Registry::GetComponent(EntityId entity)
    {
        ComponentRegistry<C>* registry = AddRegistry<C>();
        return registry->GetComponent(entity);
    }

    template<ComponentClass C>
    bool Registry::HasComponent(EntityId entity)
    {
        ComponentRegistry<C>* registry = AddRegistry<C>();
        return registry->HasComponent(entity);
    }

    template<ComponentClass C>
    void Registry::RemoveComponent(EntityId entity)
    {
        ComponentRegistry<C>* registry = AddRegistry<C>();
        return registry->RemoveComponent(entity);
    }

    template<ComponentClass C>
    ComponentRegistry<C>* Registry::AddRegistry()
    {
        if(ComponentRegistry<C>* registry = GetRegistry<C>()){return registry;}

        size_t hashCode = typeid(C).hash_code();
        registries.insert(std::make_pair(hashCode,std::unique_ptr<IComponentRegistry>(new ComponentRegistry<C>())));

        return GetRegistry<C>();
    }

    template<ComponentClass C>
    ComponentRegistry<C>* Registry::GetRegistry()
    {
        if(!HasRegistry<C>()){return nullptr;}

        size_t hashCode = typeid(C).hash_code();

       return reinterpret_cast<ComponentRegistry<C>*>(registries.at(hashCode).get());
    }

    template<ComponentClass C>
    bool Registry::HasRegistry()
    {
        size_t hashCode = typeid(C).hash_code();
        return registries.contains(hashCode);
    }
}
