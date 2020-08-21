cmake_minimum_required(VERSION 3.10)

project(PolarVortex)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(PolarVortex src/main.cpp src/memory/Memory.cpp src/memory/StackAllocator.cpp src/assets/AssetManager.cpp src/render/glad.c src/game/Window.cpp src/render/GLRenderer.cpp src/render/GLMesh.cpp src/render/GLShaderProgram.cpp src/render/GLTexture.cpp src/render/stb_image.c src/render/Math.cpp src/game/Screen.cpp src/game/Viewport.cpp src/screens/MainMenuScreen.cpp src/game/PolarVortexGame.cpp src/world/World.cpp src/screens/WorldScreen.cpp src/world/Collision.cpp src/input/GLFWGamepadDevice.cpp src/input/GLFWGamepadButtonElement.cpp src/input/GLFWGamepadAxisElement.cpp src/render/SpriteAnimation.cpp src/render/SpriteInstance.cpp src/ui/UIElement.cpp src/ui/UIAnimation.cpp src/ui/UIDockContainer.cpp src/ui/UISprite.cpp src/render/SpriteFont.cpp src/ui/UITextBlock.cpp)

if(WIN32)
    target_link_libraries(PolarVortex opengl32 ${CMAKE_SOURCE_DIR}/lib/win64-vc2019/glfw3.lib)
elseif(APPLE)
    find_library(LIBRARY_COCOA Cocoa)
    find_library(LIBRARY_OPENGL OpenGL)
    find_library(LIBRARY_IOKIT IOKit)
    target_link_libraries(PolarVortex ${CMAKE_SOURCE_DIR}/lib/macos-clang/libglfw3.a ${LIBRARY_COCOA} ${LIBRARY_OPENGL} ${LIBRARY_IOKIT})
else()
    target_link_libraries(PolarVortex GL)
endif()

file(COPY assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})