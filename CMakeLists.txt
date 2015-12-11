cmake_minimum_required(VERSION 2.8)

# use C++ 11
set(CMAKE_CXX_STANDARD 11)

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

# Include SDL2 and extension libraries
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIR}
    ${SDLMIXER_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${SDL2_TTF_INCLUDE_DIR}
    )
target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARY}
    ${SDLMIXER_LIBRARY}
    ${SDL2_IMAGE_LIBRARY}
    ${SDL2_TTF_LIBRARY}
    )
