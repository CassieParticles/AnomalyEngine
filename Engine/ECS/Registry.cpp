#include "Registry.h"

#include "Component.h"
#include "Entity.h"

namespace Engine
{
    std::unordered_map<size_t,std::unique_ptr<IComponentRegistry>> Registry::registries;
} // Engine