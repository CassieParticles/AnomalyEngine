#pragma once
#include <memory>
#include <unordered_map>

#include "ComponentConcept.h"
#include "ComponentRegistry.h"

namespace Engine
{
    class IComponentRegistry;

    class Registry
    {
    public:
        template<ComponentClass C, typename... Args>
        static C* AddComponent(EntityId entity, Args... args);

        template<ComponentClass C>
        static void RemoveComponent(EntityId entity);

        template<ComponentClass C>
        static C* GetComponent(EntityId entity);

        template<ComponentClass C>
        static bool HasComponent(EntityId entity);
    private:
        static std::unordered_map<size_t, std::unique_ptr<IComponentRegistry>> componentRegistries;

        template<ComponentClass C>
        static ComponentRegistry<C>* AddRegistry();

        template<ComponentClass C>
        static ComponentRegistry<C>* GetRegistry();

        template<ComponentClass C>
        static bool HasRegistry();

    };

    template<ComponentClass C, typename ... Args>
    C * Registry::AddComponent(EntityId entity, Args... args)
    {
        //Gets registry if it already exists
        ComponentRegistry<C>* registry = AddRegistry<C>();

        return registry->AddComponent(entity,args...);
        return nullptr;
    }

    template<ComponentClass C>
    void Registry::RemoveComponent(EntityId entity)
    {
        if (auto registry = GetRegistry<C>()){registry->RemoveComponent(entity);}
    }

    template<ComponentClass C>
    C * Registry::GetComponent(EntityId entity)
    {
        if (auto registry = GetRegistry<C>()){return dynamic_cast<C*>(registry->GetComponent(entity));}
        return nullptr;
    }

    template<ComponentClass C>
    bool Registry::HasComponent(EntityId entity)
    {
        if (auto registry = GetRegistry<C>()){return registry->HasComponent(entity);}
        return false;
    }

    template<ComponentClass C>
    ComponentRegistry<C>* Registry::AddRegistry()
    {
        if (auto registry = GetRegistry<C>())
        {
            return registry;
        }

        size_t compHash = typeid(C).hash_code();

        componentRegistries.insert(std::make_pair(compHash, std::unique_ptr<IComponentRegistry>(new ComponentRegistry<C>())));

        return GetRegistry<C>();
        return nullptr;
    }

    template<ComponentClass C>
    ComponentRegistry<C> * Registry::GetRegistry()
    {
        if (!HasRegistry<C>()){return nullptr;}
        size_t compHash = typeid(C).hash_code();
        return dynamic_cast<ComponentRegistry<C>*>(componentRegistries.at(compHash).get());
        return nullptr;
    }

    template<ComponentClass C>
    bool Registry::HasRegistry()
    {
        size_t compHash = typeid(C).hash_code();
        return componentRegistries.contains(compHash);
    }
}
