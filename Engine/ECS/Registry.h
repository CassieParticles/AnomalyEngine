#pragma once
#include <memory>
#include <unordered_map>
#include <refl.hpp>

#include "ComponentConcept.h"
#include "ComponentRegistry.h"

namespace Engine
{
    class Entity;

    struct ComponentType
    {
        ComponentType(std::string name):name{std::move(name)} {}
        std::string name;
        IComponentRegistry* registry;
        std::vector<ComponentType*> children;
    };

    class Registry
    {
    public:
        static Entity CreateEntity(const std::string& name = "Game Entity");

        template<ComponentClass C,typename... Args>
        static CompPtr<C> AddComponent(EntityId entity,Args... args);
        template<ComponentClass C>
        static CompPtr<C> GetComponentExact(EntityId entity);
        template<ComponentClass C>
        static bool HasComponentExact(EntityId entity);
        template<ComponentClass C>
        static void RemoveComponent(EntityId entity);

        template<ComponentClass C>
        static CompPtr<C> GetComponent(EntityId entity);
        template<ComponentClass C>
        static bool HasComponent(EntityId entity);
    protected:
        //Component registries
        template<ComponentClass C>
        static ComponentRegistry<C>* AddRegistry();
        template<ComponentClass C>
        static ComponentRegistry<C>* GetRegistry();
        template<ComponentClass C>
        static bool HasRegistry();

        static CompPtrInternal* GetComponentRecurse(EntityId entity, ComponentType* type);
        static bool HasComponentRecurse(EntityId entity, ComponentType* type);

        template<ComponentClass C>
        static void AddType();
        template<ComponentClass C>
        static ComponentType* GetType();
        template<ComponentClass C>
        static bool HasType();
        template<ComponentClass C>
        static void WalkParents(ComponentType* type, refl::type_descriptor<C> descriptor = refl::reflect<C>());

        static std::unordered_map<size_t, std::unique_ptr<IComponentRegistry>> registries;
        static std::unordered_map<std::string, ComponentType> componentTypes;
        static EntityId nextFreeId;
    };

    template<ComponentClass C, typename ... Args>
    CompPtr<C> Registry::AddComponent(EntityId entity, Args... args)
    {
        ComponentRegistry<C>* registry = AddRegistry<C>();
        CompPtrInternal* internal = registry->AddComponent(entity, args...);
        return CompPtr<C>(internal);
    }

    template<ComponentClass C>
    CompPtr<C> Registry::GetComponentExact(EntityId entity)
    {
        ComponentRegistry<C>* registry = AddRegistry<C>();
        CompPtrInternal* internal = registry->GetComponent(entity);
        return CompPtr<C>(internal);
    }

    template<ComponentClass C>
    bool Registry::HasComponentExact(EntityId entity)
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
    CompPtr<C> Registry::GetComponent(EntityId entity)
    {
        ComponentType* type = GetType<C>();

        CompPtrInternal* internal = GetComponentRecurse(entity,type);
        return CompPtr<C>(internal);
    }

    template<ComponentClass C>
    bool Registry::HasComponent(EntityId entity)
    {
        ComponentType* type = GetType<C>();
        return HasComponentRecurse(entity,type);
    }

    template<ComponentClass C>
    ComponentRegistry<C>* Registry::AddRegistry()
    {
        if(ComponentRegistry<C>* registry = GetRegistry<C>()){return registry;}

        size_t hashCode = typeid(C).hash_code();
        registries.insert(std::make_pair(hashCode,std::unique_ptr<IComponentRegistry>(new ComponentRegistry<C>())));

        AddType<C>();

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

    template<ComponentClass C>
    void Registry::AddType()
    {
        if(HasType<C>())
        {
            ComponentType* type = GetType<C>();
            type->registry=GetRegistry<C>();
            return;
        }

        refl::type_descriptor<C> descriptor;
        std::string name = descriptor.name.str();

        componentTypes.insert(std::make_pair(name,ComponentType(name)));
        ComponentType* type = GetType<C>();

        type->registry = GetRegistry<C>();

        WalkParents<C>(type);
    }

    template<ComponentClass C>
    ComponentType* Registry::GetType()
    {
        refl::type_descriptor<C> type;
        return &componentTypes.at(type.name.str());
    }

    template<ComponentClass C>
    bool Registry::HasType()
    {
        refl::type_descriptor<C> type;
        std::string name = type.name.str();
        return componentTypes.contains(name);
    }

    template<ComponentClass C>
    void Registry::WalkParents(ComponentType* type, refl::type_descriptor<C> descriptor)
    {
        refl::util::for_each(refl::util::reflect_types(descriptor.declared_bases),[&](auto x)
        {
            //Find parent type (insert if doesn't exist)
            auto parent = componentTypes.find(x.name.str());
            if(parent==componentTypes.end())
            {
                componentTypes.insert(std::make_pair(x.name.str(),ComponentType(x.name.str())));
                parent = componentTypes.find(x.name.str());

                parent->second.registry = nullptr;
            }

            //Check if parent has child registered
            bool found=false;
            for(auto it:parent->second.children)
            {
                if(it==type)
                {
                    found=true;
                    break;
                }
            }
            //If child is unregistered, register child and WalkParents of parent class
            if(!found)
            {
                parent->second.children.push_back(type);
                WalkParents(&parent->second,x);
                return;
            }
        });
    }
}
