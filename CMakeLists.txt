cmake_minimum_required(VERSION 3.1)
project(game1)

# @TODO project structure:
# split all src into 3 dirs: client, server, common

# Source files
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(UTIL_DIR "${SRC_DIR}/utils")
configure_file("${UTIL_DIR}/paths.h.in" "${UTIL_DIR}/paths.h")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCES 
    "${SRC_DIR}/main.cpp"
    "${SRC_DIR}/Game.cpp"
    "${SRC_DIR}/Camera.cpp"
    "${SRC_DIR}/Player.cpp"
    "${SRC_DIR}/Block.cpp"
    "${SRC_DIR}/ShaderManager.cpp"
    "${SRC_DIR}/GLResourceManager.cpp"
    "${SRC_DIR}/Grid.cpp"
    "${SRC_DIR}/Client.cpp"
    "${SRC_DIR}/physics/PhysicsBody.cpp"
    "${SRC_DIR}/collisions.cpp"
    "${SRC_DIR}/Boid.cpp"
    "${SRC_DIR}/BoidSwarm.cpp"
    "${SRC_DIR}/TextRenderer.cpp"
    "${SRC_DIR}/InputManager.cpp"

    "${SRC_DIR}/server/Server.cpp"
    
    "${UTIL_DIR}/networking.cpp"
    "${UTIL_DIR}/CircleFilled.cpp"
    #"${UTIL_DIR}/Circle.cpp"
    "${UTIL_DIR}/Rectangle.cpp"


    "${SRC_DIR}/ui/UIElement.cpp"
    "${SRC_DIR}/ui/UIContainer.cpp"
    "${SRC_DIR}/ui/WindowElement.cpp"
    "${SRC_DIR}/ui/TextElement.cpp"
    "${SRC_DIR}/ui/ValueSliderElement.cpp"
    "${SRC_DIR}/ui/SliderBarElement.cpp"
)

set(INCLUDES 
    "${SRC_DIR}/Game.h"
    "${SRC_DIR}/Camera.h"
    "${SRC_DIR}/Player.h" 
    "${SRC_DIR}/Block.h" 
    "${SRC_DIR}/ShaderManager.h" 
    "${SRC_DIR}/GLResourceManager.h" 
    "${SRC_DIR}/Grid.h" 
    "${SRC_DIR}/Client.h" 
    "${SRC_DIR}/DebugOptions.h" 
    "${SRC_DIR}/physics/PhysicsBody.h" 
    "${SRC_DIR}/collisions.h"
    "${SRC_DIR}/Boid.h"
    "${SRC_DIR}/BoidSwarm.h"
    "${SRC_DIR}/TextRenderer.h"
    "${SRC_DIR}/InputManager.h"

    "${SRC_DIR}/server/Server.h"

    "${UTIL_DIR}/networking.h" 
    "${UTIL_DIR}/CircleFilled.h"
    #"${UTIL_DIR}/Circle.h"
    "${UTIL_DIR}/Rectangle.h"

    "${SRC_DIR}/ui/UIElement.h"
    "${SRC_DIR}/ui/UIContainer.h"
    "${SRC_DIR}/ui/WindowElement.h"
    "${SRC_DIR}/ui/TextElement.h"
    "${SRC_DIR}/ui/ValueSliderElement.h"
    "${SRC_DIR}/ui/SliderBarElement.h"
)

# Executable definition and properties
add_executable(${PROJECT_NAME} ${SOURCES} ${INCLUDES})
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")
target_include_directories(${PROJECT_NAME} PRIVATE "${INCLUDE_DIR}")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

# glm
set(GLM_INCLUDE_DIR "${INCLUDE_DIR}/glm")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLM_INCLUDE_DIR}")

# GLFW
set(GLFW_DIR "${LIB_DIR}/glfw-3.3.2")
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory("${GLFW_DIR}")
target_link_libraries(${PROJECT_NAME} "glfw" "${GLFW_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

# glad
set(GLAD_DIR "${LIB_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")


# TODO can compile a much leaner version of freetype apparantly to just do ttf stuff
# freetype
set(FREETYPE_DIR "${LIB_DIR}/freetype-2.10.2")
add_subdirectory("${FREETYPE_DIR}")
target_include_directories(${PROJECT_NAME} PRIVATE "${FREETYPE_DIR}/include/freetype")
target_link_libraries(${PROJECT_NAME} "freetype" "${CMAKE_DL_LIBS}")