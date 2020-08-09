cmake_minimum_required(VERSION 3.10)

project(PolarVortex)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(PolarVortex src/main.cpp src/memory/Memory.cpp src/memory/StackAllocator.cpp src/assets/AssetManager.cpp src/render/glad.c src/game/Window.cpp src/render/GLRenderer.cpp src/render/GLMesh.cpp src/render/GLShaderProgram.cpp)

if(WIN32)
    target_link_libraries(PolarVortex opengl32 ${CMAKE_SOURCE_DIR}/lib/win64-vc2019/glfw3.lib)
elseif(APPLE)
    # TODO: Implement
else()
    target_link_libraries(PolarVortex GL)
endif()

file(COPY assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})