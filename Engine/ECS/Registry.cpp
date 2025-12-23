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
} // Engine