cmake_minimum_required(VERSION 2.8)

# use C++ 11
set(CMAKE_CXX_FLAGS "-std=c++11")

set(PROJECT_NAME "ASCIILib")
set(SRC_DIR "ASCIILib")

project(${PROJECT_NAME})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${ASCIILib_SOURCE_DIR}/cmake")

# find all sources in the source directory
file(GLOB ASCIILib_src
    "${SRC_DIR}/*.h"
    "${SRC_DIR}/*.cpp"
    )

add_library(${PROJECT_NAME} ${ASCIILib_src})

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIR}
    ${SDL2MIXER_INCLUDE_DIR}
    ${SDL2IMAGE_INCLUDE_DIR}
    ${SDL2TTF_INCLUDE_DIR}
    )
target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARIES}
    ${SDL2MIXER_LIBRARIES}
    ${SDL2IMAGE_LIBRARIES}
    ${SDL2TTF_LIBRARIES}
    )
