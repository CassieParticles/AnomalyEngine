#pragma once
#include <cstdint>
#include <type_traits>

namespace Engine
{
    class Component;

    template<typename T>
    concept ComponentClass = std::is_base_of_v<Component, T>;

    typedef uint32_t EntityId;
}
