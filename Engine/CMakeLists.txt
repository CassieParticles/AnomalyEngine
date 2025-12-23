set(CMAKE_CXX_STANDARD 20)

add_library(AnomalyEngine
        EntryPoint/Application.cpp
        EntryPoint/Application.h
        ECS/ComponentConcept.h
        ECS/Component.cpp
        ECS/Component.h
        ECS/Entity.cpp
        ECS/Entity.h
        ECS/ComponentRegistry.cpp
        ECS/ComponentRegistry.h
        ECS/Registry.cpp
        ECS/Registry.h
        ECS/CompPtr.h
)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_definitions(AnomalyEngine PUBLIC "DEBUG")
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
    target_compile_definitions(AnomalyEngine PUBLIC "RELEASE")
endif()

target_include_directories(AnomalyEngine PUBLIC libraries/refl)