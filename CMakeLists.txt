cmake_minimum_required(VERSION 3.16)

project(CompSim1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -pedantic -Wfatal-errors -fvisibility=hidden)
endif()

set(SOURCE src/main.cpp src/Spacetime.cpp src/ImageRenderer.cpp src/Schwarzschild.cpp)

add_executable(test ${SOURCE})
