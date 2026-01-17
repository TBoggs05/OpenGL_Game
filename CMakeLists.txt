cmake_minimum_required(VERSION 3.26.0)
project(opengl_game VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(opengl_game
    src/main.cpp 
    src/glad.c  

)

target_include_directories(opengl_game PRIVATE dependencies)

target_link_libraries(opengl_game ${GLFW3_LIBRARY} OpenGL::GL)