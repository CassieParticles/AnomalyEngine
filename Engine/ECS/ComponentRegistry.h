#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

#include "ComponentConcept.h"
#include "CompPtr.h"

namespace Engine
{
    class Registry;


    class IComponentRegistry
    {
        friend class Registry;
    public:
        virtual void RemoveComponent(EntityId entity) = 0;
        virtual bool HasComponent(EntityId entity) = 0;
        virtual Component* GetComponent(EntityId entity) = 0;
    protected:
        IComponentRegistry() = default;

        //Ways for derived templates to access private component members, since generic friend classes won't work
        void OnComponentAdded(EntityId entity, Component* component);
        void OnComponentCopied(EntityId entity, Component* component);
        void OnComponentRemoved(EntityId entity, Component* component);

        virtual void resize() = 0;
    };

    template<ComponentClass C>
    class ComponentRegistry final:public IComponentRegistry
    {
        friend class Registry;
        public:
        template<typename... Args>
        CompPtr<C> AddComponent(EntityId entity, Args... args);
        void RemoveComponent(EntityId entity) override;
        bool HasComponent(EntityId entity) override;
        Component* GetComponent(EntityId entity) override;

        CompPtr<C> GetCompPtr(EntityId entity);

    protected:
        ComponentRegistry();

        //Storing the components
        std::vector<uint8_t> componentBuffer;
        size_t componentCount;

        //The index of each component attached to Entity
        std::unordered_map<EntityId, size_t> entityIndexMap;

        //Smart pointers for components
        CompPtrInternal<C>* AddInternal(EntityId entity);
        CompPtrInternal<C>* GetInternal(EntityId entity);
        CompPtrInternal<C>* UpdateInternal(EntityId entity);
        void UpdateAllInternals();
        std::unordered_map<EntityId, CompPtrInternal<C>> CompPtrs;

        void resize() override;
    };

    template<ComponentClass C>
    template<typename ... Args>
    CompPtr<C> ComponentRegistry<C>::AddComponent(EntityId entity, Args... args)
    {
        //If component already exists, return
        if (Component* component = GetComponent(entity))
        {
#ifdef DEBUG
            std::cout<<"Warning, attempt to create component failed: Component already exists\n";
#endif
            return GetCompPtr(entity);
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

        AddInternal(entity);

        return GetCompPtr(entity);
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

            UpdateInternal(finalEntity);
        }

        //Remove the entity
        entityIndexMap.erase(entity);
        componentCount--;

        UpdateInternal(entity);
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
    CompPtr<C> ComponentRegistry<C>::GetCompPtr(EntityId entity)
    {
        CompPtrInternal<C>* ptrInternal = AddInternal(entity);

        return CompPtr<C>(ptrInternal);
    }

    template<ComponentClass C>
    ComponentRegistry<C>::ComponentRegistry():componentCount(0)
    {
        componentBuffer.resize(1024);
    }

    template<ComponentClass C>
    CompPtrInternal<C> * ComponentRegistry<C>::AddInternal(EntityId entity)
    {
        if (CompPtrs.contains(entity)){return &CompPtrs.at(entity);}

        size_t componentIndex = entityIndexMap.at(entity);
        uint8_t* componentLocation = componentBuffer.data() + componentIndex * sizeof(C);
        C* component = reinterpret_cast<C*>(componentLocation);

        CompPtrs.insert(std::make_pair(entity, CompPtrInternal<C>(entity, component)));

        return &CompPtrs.at(entity);
    }

    template<ComponentClass C>
    CompPtrInternal<C> * ComponentRegistry<C>::GetInternal(EntityId entity)
    {
        if (CompPtrs.contains(entity)){return &CompPtrs.at(entity);}
        return nullptr;
    }

    template<ComponentClass C>
    CompPtrInternal<C> * ComponentRegistry<C>::UpdateInternal(EntityId entity)
    {
        if (!CompPtrs.contains(entity)){return nullptr;}
        CompPtrInternal<C>* ptrInternal = &CompPtrs.at(entity);

        size_t componentIndex = entityIndexMap.at(entity);
        uint8_t* componentLocation = componentBuffer.data() + componentIndex * sizeof(C);

        ptrInternal->component = reinterpret_cast<C*>(componentLocation);

        return ptrInternal;
    }

    template<ComponentClass C>
    void ComponentRegistry<C>::UpdateAllInternals()
    {
        for (auto pair: CompPtrs)
        {
            UpdateInternal(pair.first);
        }
    }

    template<ComponentClass C>
    void ComponentRegistry<C>::resize()
    {
        componentBuffer.resize(componentBuffer.size() * 2);

        UpdateAllInternals();
    }
} // Engine