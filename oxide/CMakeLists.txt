cmake_minimum_required(VERSION 3.8)

project("oxide")

add_library(oxide STATIC 
"src/oxide/oxide.cpp" "src/oxide/oxide.h"
"src/Window/Window.cpp" "src/Window/Window.h"
"src/Events/Events.cpp" "src/Events/Events.h"
"src/Renderer/Renderer.h" "src/Renderer/Renderer.cpp" "include/oxide/Renderer.h" 
"src/stb_image/stb_image.cpp"
"src/stb_image/stb_image.h")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

#glfw
add_subdirectory("external/glfw")
target_link_libraries(oxide glfw)

#glew
add_subdirectory(external/glew/build/cmake)
target_link_libraries(oxide glew_s)
target_include_directories(oxide PUBLIC external/glew/include/)

#glm
add_subdirectory(external/glm)
target_link_libraries(oxide glm)

#nlohamnn json
include_directories(external/json)

target_include_directories(oxide PUBLIC "include/")