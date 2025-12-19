set(CMAKE_CXX_STANDARD 20)

add_library(AnomalyEngine
        Application.cpp
        Application.h
)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_definitions(AnomalyEngine PUBLIC "DEBUG")
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
    target_compile_definitions(AnomalyEngine PUBLIC "RELEASE")
endif()