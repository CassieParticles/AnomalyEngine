#include "Registry.h"

#include "Component.h"
#include "Entity.h"

namespace Engine
{
    std::unordered_map<size_t, std::unique_ptr<IComponentRegistry>> Registry::componentRegistries;
    EntityId Registry::nextFreeId = 0;

    Entity Registry::CreateEntity(const std::string &name)
    {
        Entity entity = nextFreeId++;

        //Add tag component
        AddComponent<TagComponent>(entity, name);


        return entity;
    }


} // Engine