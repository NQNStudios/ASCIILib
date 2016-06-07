cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME "ASCIILib")
set(SRC_DIR "ASCIILib")

project(${PROJECT_NAME})

if (UNIX)
    # Use C++11
    set(CMAKE_CXX_FLAGS "-std=c++11")
    set(CMAKE_CXX_STANDARD 11)
else()
    # Define windows variables
    SET(ENV{SDL2DIR} "${ASCIILib_SOURCE_DIR}")
    SET(ENV{SDL2_IMAGE} "${ASCIILib_SOURCE_DIR}")
    SET(ENV{SDL2_TTF} "${ASCIILib_SOURCE_DIR}")
    SET(ENV{SDL2_MIXER_HOME} "${ASCIILib_SOURCE_DIR}")
    SET(ENV{ICU_ROOT} "${ASCIILib_SOURCE_DIR}")
endif(UNIX)


# find all sources in the source directory
file(GLOB ASCIILib_src
    "${SRC_DIR}/*.h"
    "${SRC_DIR}/*.cpp"
    )

add_library(${PROJECT_NAME} ${ASCIILib_src})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${ASCIILib_SOURCE_DIR}/cmake")

# Include SDL2 and extension libraries, plus ICU
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

find_package(ICU COMPONENTS uc io REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIR}
    ${SDLMIXER_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${SDL2_TTF_INCLUDE_DIR}
    ${ICU_INCLUDE_DIRS}
    )
target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARY}
    ${SDLMIXER_LIBRARY}
    ${SDL2_IMAGE_LIBRARY}
    ${SDL2_TTF_LIBRARY}
    ${ICU_LIBRARIES}
    )
