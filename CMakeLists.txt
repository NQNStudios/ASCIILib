cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME "ASCIILib")
set(SRC_DIR "ASCIILib")

project(${PROJECT_NAME})

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.7)

if (UNIX)
    # Use C++11
    set(CMAKE_CXX_FLAGS "-std=c++11")
    set(CMAKE_CXX_STANDARD 11)
else()
    # Define windows variables
    SET(ENV{SDL2DIR} "${ASCIILib_SOURCE_DIR}")
    SET(ENV{SDL2_IMAGE} "${ASCIILib_SOURCE_DIR}")
    SET(ENV{SDL2_MIXER} "${ASCIILib_SOURCE_DIR}")
    SET(ENV{ICU_ROOT} "${ASCIILib_SOURCE_DIR}")

    # Define preprocessor definitions to avoid unwanted windows includes
    add_definitions(-DNOGDI)
    add_definitions(-DNOMINMAX)
endif(UNIX)


# find all sources in the source directory
SET(ASCIILib_src
    "${SRC_DIR}/Camera.cpp"
    "${SRC_DIR}/Camera.h"
    "${SRC_DIR}/Color.cpp"
    "${SRC_DIR}/Color.h"
    "${SRC_DIR}/ContentManager.cpp"
    "${SRC_DIR}/ContentManager.h"
    "${SRC_DIR}/DialogFrame.cpp"
    "${SRC_DIR}/DialogFrame.h"
    "${SRC_DIR}/DialogScene.cpp"
    "${SRC_DIR}/DialogScene.h"
    "${SRC_DIR}/DialogStyle.cpp"
    "${SRC_DIR}/DialogStyle.h"
    "${SRC_DIR}/FilePaths.cpp"
    "${SRC_DIR}/FilePaths.h"
    "${SRC_DIR}/FileReader.cpp"
    "${SRC_DIR}/FileReader.h"
    "${SRC_DIR}/Game.cpp"
    "${SRC_DIR}/Game.h"
    "${SRC_DIR}/GlobalArgs.cpp"
    "${SRC_DIR}/GlobalArgs.h"
    "${SRC_DIR}/Graphics.cpp"
    "${SRC_DIR}/Graphics.h"
    "${SRC_DIR}/ImageCache.cpp"
    "${SRC_DIR}/ImageCache.h"
    "${SRC_DIR}/Input.cpp"
    "${SRC_DIR}/Input.h"
    "${SRC_DIR}/LanguageManager.cpp"
    "${SRC_DIR}/LanguageManager.h"
    "${SRC_DIR}/Log.cpp"
    "${SRC_DIR}/Log.h"
    "${SRC_DIR}/Log.tpp"
    "${SRC_DIR}/PixelFont.cpp"
    "${SRC_DIR}/PixelFont.h"
    "${SRC_DIR}/Point.cpp"
    "${SRC_DIR}/Point.h"
    "${SRC_DIR}/Preferences.cpp"
    "${SRC_DIR}/Preferences.h"
    "${SRC_DIR}/Rectangle.cpp"
    "${SRC_DIR}/Rectangle.h"
    "${SRC_DIR}/ScrollingWord.cpp"
    "${SRC_DIR}/ScrollingWord.h"
    "${SRC_DIR}/SoundManager.cpp"
    "${SRC_DIR}/SoundManager.h"
    "${SRC_DIR}/State.h"
    "${SRC_DIR}/StringTokenizer.cpp"
    "${SRC_DIR}/StringTokenizer.h"
    "${SRC_DIR}/StyleManager.cpp"
    "${SRC_DIR}/StyleManager.h"
    "${SRC_DIR}/Surface.cpp"
    "${SRC_DIR}/Surface.h"
    "${SRC_DIR}/SurfaceManager.cpp"
    "${SRC_DIR}/SurfaceManager.h"
    "${SRC_DIR}/TextManager.cpp"
    "${SRC_DIR}/TextManager.h"
    "${SRC_DIR}/Tween.cpp"
    "${SRC_DIR}/Tween.h"
    "${SRC_DIR}/content.cpp"
    "${SRC_DIR}/content.h"
    "${SRC_DIR}/InputAction.cpp"
    "${SRC_DIR}/InputAction.h"
    "${SRC_DIR}/InputMappings.cpp"
    "${SRC_DIR}/InputMappings.h"
    "${SRC_DIR}/json-forwards.h"
    "${SRC_DIR}/json.h"
    "${SRC_DIR}/jsoncpp.cpp"
    "${SRC_DIR}/json.h"
    "${SRC_DIR}/Alignment.h"
    "${SRC_DIR}/Button.cpp"
    "${SRC_DIR}/Button.h"
    "${SRC_DIR}/Label.cpp"
    "${SRC_DIR}/Label.h"
    "${SRC_DIR}/Menu.cpp"
    "${SRC_DIR}/Menu.h"
    "${SRC_DIR}/parsing.h"
    )

add_library(${PROJECT_NAME} ${ASCIILib_src})
message("Creating library" ${PROJECT_NAME})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${ASCIILib_SOURCE_DIR}/cmake")

# Include SDL2 and extension libraries, plus ICU
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_image REQUIRED)

find_package(ICU COMPONENTS uc io REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${ICU_INCLUDE_DIRS}
    )
target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARY}
    ${SDL2_MIXER_LIBRARY}
    ${SDL2_IMAGE_LIBRARY}
    ${ICU_LIBRARIES}
    )
