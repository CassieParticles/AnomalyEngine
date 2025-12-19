cmake_minimum_required(VERSION 4.1)
project(AnomalyEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(Entry Entry.cpp)

#Add Game
add_subdirectory(Game)
target_link_libraries(Entry PUBLIC Game)
target_include_directories(Entry PUBLIC Game)