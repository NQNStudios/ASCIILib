cmake_minimum_required(VERSION 2.8)

# use C++ 11
set(CMAKE_CXX_FLAGS "-std=c++11")

set(PROJECT_NAME "ASCIILib")

project(${PROJECT_NAME})

# find all sources in this directory
file(GLOB ASCIILib_src
    "*.h"
    "*.cpp"
    )

add_library(${PROJECT_NAME} ${ASCIILib_src})

include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2MIXER_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2MIXER_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
