#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

#include "ComponentConcept.h"

namespace Engine
{
    class IComponentRegistry
    {
    public:
        virtual void RemoveComponent(EntityId entity) = 0;
        virtual bool HasComponent(EntityId entity) = 0;
        virtual Component* GetComponent(EntityId entity) = 0;
        IComponentRegistry() = default;
    protected:

        //Ways for derived templates to access private component members, since generic friend classes won't work
        void OnComponentAdded(EntityId entity, Component* component);
        void OnComponentCopied(EntityId entity, Component* component);
        void OnComponentRemoved(EntityId entity, Component* component);

        virtual void resize() = 0;
    };

    template<ComponentClass C>
    class ComponentRegistry final:public IComponentRegistry
    {
        public:
        template<typename... Args>
        C* AddComponent(EntityId entity, Args... args);
        void RemoveComponent(EntityId entity) override;
        bool HasComponent(EntityId entity) override;
        Component* GetComponent(EntityId entity) override;

        ComponentRegistry();
    protected:

        //Storing the components
        std::vector<uint8_t> componentBuffer;
        size_t componentCount;

        //The index of each component attached to Entity
        std::unordered_map<EntityId, size_t> entityIndexMap;

        void resize() override;
    };

    template<ComponentClass C>
    template<typename ... Args>
    C * ComponentRegistry<C>::AddComponent(EntityId entity, Args... args)
    {
        //If component already exists, return
        if (Component* component = GetComponent(entity))
        {
#ifdef DEBUG
            std::cout<<"Warning, attempt to create component failed: Component already exists\n";
#endif
            return static_cast<C*>(component);
        }
        //Check whether buffer needs to be resized
        if ((componentCount + 1) * sizeof(C) > componentBuffer.size())
        {
            resize();
        }

        //Create new component using placement new operator
        uint8_t* newComponentLocation = componentBuffer.data() + componentCount * sizeof(C);
        C* newComponent = new(newComponentLocation) C(args...);

        //Attach entity ID to new component
        OnComponentAdded(entity, newComponent);
        entityIndexMap.insert(std::make_pair(entity, componentCount));

        //Update next free pointer
        componentCount++;

        //Return new component
        return newComponent;
    }

    template<ComponentClass C>
    void ComponentRegistry<C>::RemoveComponent(EntityId entity)
    {
        int componentIndex = entityIndexMap.at(entity);
        C* component = reinterpret_cast<C*>(GetComponent(entity));
        if (!component)
        {
            return;
        }

        //Call remove component functions & destructor
        OnComponentRemoved(entity,component);
        component->~C();

        //If component is not final component, copy final component into component location
        if (componentIndex != componentCount - 1)
        {
            uint8_t* componentLocation = reinterpret_cast<uint8_t*>(component);
            uint8_t* finalComponentLocation = componentBuffer.data() + (componentCount - 1) * sizeof(C);

            //Get the final component's entity id
            EntityId finalEntity = reinterpret_cast<C*>(finalComponentLocation)->getEntity();

            //Copy the component
            memcpy(componentLocation, finalComponentLocation, sizeof(C));

            //Update the final component's map entry
            entityIndexMap.at(finalEntity) = componentIndex;
        }

        //Remove the entity
        entityIndexMap.erase(entity);
        componentCount--;

    }

    template<ComponentClass C>
    bool ComponentRegistry<C>::HasComponent(EntityId entity)
    {
        return entityIndexMap.contains(entity);
    }

    template<ComponentClass C>
    Component* ComponentRegistry<C>::GetComponent(EntityId entity)
    {
        auto compIter = entityIndexMap.find(entity);
        if (compIter == entityIndexMap.end()) { return nullptr;}

        uint8_t* componentLocation = componentBuffer.data() + compIter->second * sizeof(C);
        auto component = reinterpret_cast<Component*>(componentLocation);

        return component;
    }

    template<ComponentClass C>
    ComponentRegistry<C>::ComponentRegistry():componentCount(0)
    {
        componentBuffer.resize(1024);
    }

    template<ComponentClass C>
    void ComponentRegistry<C>::resize()
    {
        componentBuffer.resize(componentBuffer.size() * 2);
    }
} // Engine