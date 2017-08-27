cmake_minimum_required(VERSION 2.8.4)
project(Cosmos_Chip8_Interpreter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES cosmos.cpp cpu.cpp)
add_executable(cosmos ${SOURCE_FILES})

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(cosmos ${SDL2_LIBRARIES})
