#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

#include "ComponentConcept.h"
#include "CompPtr.h"

namespace Engine
{
    class IComponentRegistry
    {
    public:
        virtual ~IComponentRegistry() = default;
    protected:
        //Internal utility functions
        virtual uint8_t* GetPtrIdx(size_t index) = 0;
        virtual uint8_t* GetPtrEnt(EntityId entity) = 0;
        virtual bool GetPtrExistIdx(size_t index) = 0;
        virtual bool GetPtrExistEnt(EntityId entity) = 0;

        virtual void MoveCompLoc(size_t oldIndex, size_t newIndex) = 0;
        virtual void DelComp(EntityId entity) = 0;

        void OnComponentAdded(EntityId entity, Component* component);
        void OnComponentRemoved(EntityId entity, Component* component);
    };

    template<ComponentClass C>
    class ComponentRegistry final: IComponentRegistry
    {
    public:
        template<typename... Args>
        CompPtr<C>&& AddComponent(EntityId entity, Args... args);
        CompPtr<C>&& GetComponent(EntityId entity);
        bool HasComponent(EntityId entity);
        void DeleteComponent(EntityId entity);
    protected:
        uint8_t* GetPtrIdx(size_t index) override;
        uint8_t* GetPtrEnt(EntityId entity) override;
        bool GetPtrExistIdx(size_t index) override { return index < nextFreeIdx; }
        bool GetPtrExistEnt(EntityId entity) override { return entIdxMap.contains(entity); }

        void MoveCompLoc(size_t oldIndex, size_t newIndex) override;
        void DelComp(EntityId entity) override;

        //Buffer for storing components
        std::vector<uint8_t> componentBuffer;
        //Map between entityId and index of component
        std::unordered_map<EntityId, size_t> entIdxMap;
        //Next free index
        size_t nextFreeIdx{};

        void AddInternal(EntityId entity);
        void UpdateInternal(EntityId entity);
        void UpdateInternals();

        bool HasInternal(EntityId entity){return compPtrInternals.contains(entity);}
        CompPtrInternal<C>* GetInternal(EntityId entity){return compPtrInternals.at(entity);}

        //Smart pointer internals, used to track where components point to
        std::unordered_map<EntityId, CompPtrInternal<C>> compPtrInternals;
    };


    template<ComponentClass C>
    template<typename ... Args>
    CompPtr<C>&& ComponentRegistry<C>::AddComponent(EntityId entity, Args... args)
    {
        if(GetPtrExistEnt(entity)){return GetComponent(entity);}

        //TODO: Check if resize is needed

        //Create new component in location
        uint8_t* location = GetPtrEnt(nextFreeIdx);
        C* newComponent = new(location) C(args...);

        //Attach entity
        entIdxMap.insert(std::make_pair(entity, nextFreeIdx));
        OnComponentAdded(entity, newComponent);

        nextFreeIdx++;

        AddInternal(entity);

        return GetComponent(newComponent);
    }

    template<ComponentClass C>
    CompPtr<C>&& ComponentRegistry<C>::GetComponent(EntityId entity)
    {
        CompPtrInternal<C>* ptr = GetInternal(entity);
        return CompPtr<C>{ptr};
    }

    template<ComponentClass C>
    bool ComponentRegistry<C>::HasComponent(EntityId entity)
    {
        return GetPtrEnt(entity);
    }

    template<ComponentClass C>
    void ComponentRegistry<C>::DeleteComponent(EntityId entity)
    {
        if(!GetPtrExistEnt(entity)){return;}

        DelComp(entity);

        if(entIdxMap.at(entity) != nextFreeIdx - 1)
        {
            MoveCompLoc(nextFreeIdx - 1, entIdxMap.at(entity));
        }

        entIdxMap.erase(entity);
        nextFreeIdx--;

        UpdateInternal(entity);
    }

    template<ComponentClass C>
    uint8_t* ComponentRegistry<C>::GetPtrIdx(size_t index)
    {
        return compPtrInternals.data() + index * sizeof(C);
    }

    template<ComponentClass C>
    uint8_t* ComponentRegistry<C>::GetPtrEnt(EntityId entity)
    {
        if(!GetPtrExistEnt(entity)){ return nullptr;}
        return GetPtrIdx(entIdxMap.at(entity));
    }

    template<ComponentClass C>
    void ComponentRegistry<C>::MoveCompLoc(size_t oldIndex, size_t newIndex)
    {
        uint8_t* location = GetPtrIdx(newIndex);
        uint8_t* source = GetPtrIdx(oldIndex);

        C* component = reinterpret_cast<C*>(source);

        memcpy(location, source, sizeof(C));

        entIdxMap.at(component->getEntity()) = newIndex;
    }

    template<ComponentClass C>
    void ComponentRegistry<C>::DelComp(EntityId entity)
    {
        C* component = reinterpret_cast<C*>(GetPtrEnt(entity));

        component->~C();
        OnComponentRemoved(entity, dynamic_cast<Component*>(component));
    }

    template<ComponentClass C>
    void ComponentRegistry<C>::AddInternal(EntityId entity)
    {
        if(HasInternal(entity)){return;}

        C* component = reinterpret_cast<C*>(GetPtrEnt(entity));

        compPtrInternals.insert(std::make_pair(entity, CompPtrInternal<C>{component, entity}));
    }

    template<ComponentClass C>
    void ComponentRegistry<C>::UpdateInternal(EntityId entity)
    {
        CompPtrInternal<C>* ptr = GetInternal(entity);
        ptr->component = reinterpret_cast<C*>(GetPtrEnt(entity));
    }

    template<ComponentClass C>
    void ComponentRegistry<C>::UpdateInternals()
    {
        for(auto pair:compPtrInternals)
        {
            UpdateInternal(pair.first);
        }
    }
} // Engine