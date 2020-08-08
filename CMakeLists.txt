cmake_minimum_required(VERSION 3.10)

project(PolarVortex)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(PolarVortex src/main.cpp src/memory/Memory.cpp src/memory/StackAllocator.cpp src/assets/AssetManager.cpp)

file(COPY assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})