#pragma once
#include <memory>
#include <unordered_map>

#include "ComponentConcept.h"
#include "ComponentRegistry.h"

namespace Engine
{
    class IComponentRegistry;
    class Entity;

    class Registry
    {
    public:
        static Entity CreateEntity(const std::string& name = "Game Entity");

        template<ComponentClass C, typename... Args>
        static CompPtr<C> AddComponent(EntityId entity, Args... args);

        template<ComponentClass C>
        static void RemoveComponent(EntityId entity);

        template<ComponentClass C>
        static C* GetComponent(EntityId entity);

        template<ComponentClass C>
        static bool HasComponent(EntityId entity);

        template<ComponentClass C>
        static CompPtr<C> GetCompPtr(EntityId entity);
    private:
        static std::unordered_map<size_t, std::unique_ptr<IComponentRegistry>> componentRegistries;
        static EntityId nextFreeId;

        template<ComponentClass C>
        static ComponentRegistry<C>* AddRegistry();

        template<ComponentClass C>
        static ComponentRegistry<C>* GetRegistry();

        template<ComponentClass C>
        static bool HasRegistry();

    };

    template<ComponentClass C, typename ... Args>
    CompPtr<C> Registry::AddComponent(EntityId entity, Args... args)
    {
        //Gets registry if it already exists
        ComponentRegistry<C>* registry = AddRegistry<C>();

        return registry->AddComponent(entity,args...);
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
    CompPtr<C> Registry::GetCompPtr(EntityId entity)
    {
        return dynamic_cast<ComponentRegistry<C>*>(GetRegistry<C>())->GetCompPtr(entity);
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
