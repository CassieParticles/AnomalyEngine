#include "Registry.h"

namespace Engine
{
    std::unordered_map<size_t, std::unique_ptr<IComponentRegistry>> Registry::componentRegistries;


} // Engine