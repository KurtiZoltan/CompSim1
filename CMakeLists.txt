cmake_minimum_required(VERSION 3.5)

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

file(GLOB SOURCE "src/*.cpp" "src/*.hpp")

find_package(TIFF REQUIRED)
find_package(OpenMP REQUIRED)

add_executable(test ${SOURCE})
target_link_libraries(test PRIVATE ${TIFF_LIBRARIES} PRIVATE OpenMP::OpenMP_CXX)
target_include_directories(test PRIVATE ${TIFF_INCLUDE_DIR})
