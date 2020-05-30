cmake_minimum_required(VERSION 3.16)
project(zzRenderer)

set(CMAKE_CXX_STANDARD 17)

add_executable(zzRenderer main.cpp RenderSystem.cpp RenderSystem.hpp)