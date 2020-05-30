cmake_minimum_required(VERSION 3.16)
project(zzRenderer)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(zzRenderer RenderSystem.cpp RenderSystem.hpp Vertex3.hpp Mesh.cpp Mesh.hpp)
target_link_libraries(zzRenderer SDL2::Main ${OPENGL_LIBRARIES})