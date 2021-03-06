cmake_minimum_required(VERSION 3.16)
project(zzRenderer)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/tinygltf)
set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(zzRenderer GLTFLoader.cpp RenderSystem.cpp RenderSystem.hpp Vertex3.hpp Model.cpp Model.hpp ShaderProgram.hpp ShaderProgram.cpp ShaderUniform.hpp ShaderUniform.cpp ShaderSystem.hpp ShaderSystem.cpp Material.hpp Material.cpp RenderComponent.hpp RenderComponent.cpp Texture.hpp Texture.cpp zzUtil.hpp zzUtil.cpp)

target_link_libraries(zzRenderer SDL2::Main ${OPENGL_LIBRARIES} GLEW::GLEW)