set(CMAKE_CXX_STANDARD 20)

add_library(Game
        Main.cpp
)

#Add Anomaly Engine
add_subdirectory(../Engine Engine)
target_link_libraries(Game PUBLIC AnomalyEngine)
target_include_directories(Game PUBLIC ../Engine)