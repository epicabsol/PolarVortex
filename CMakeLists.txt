cmake_minimum_required(VERSION 3.10)

project(PolarVortex)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

set (SOURCES src/memory/Memory.cpp src/memory/StackAllocator.cpp src/assets/AssetManager.cpp src/render/glad.c src/game/Window.cpp src/render/GLRenderer.cpp src/render/GLMesh.cpp src/render/GLShaderProgram.cpp src/render/GLTexture.cpp src/render/stb_image.c src/render/Math.cpp src/game/Screen.cpp src/game/Viewport.cpp src/screens/MainMenuScreen.cpp src/game/PolarVortexGame.cpp src/world/World.cpp src/screens/WorldScreen.cpp src/world/Collision.cpp src/input/GLFWGamepadDevice.cpp src/input/GLFWGamepadButtonElement.cpp src/input/GLFWGamepadAxisElement.cpp src/render/SpriteAnimation.cpp src/render/SpriteInstance.cpp src/ui/UIElement.cpp src/ui/UIAnimation.cpp src/ui/UIDockContainer.cpp src/ui/UISprite.cpp src/render/SpriteFont.cpp src/ui/UITextBlock.cpp src/world/Player.cpp src/world/TilePalette.cpp src/world/WorldBlueprint.cpp src/world/GridBlueprint.cpp src/world/Grid.cpp)

if(WIN32)
    set(LIBRARIES opengl32 ${CMAKE_SOURCE_DIR}/lib/win64-vc2019/glfw3.lib)
elseif(APPLE)
    find_library(LIBRARY_COCOA Cocoa)
    find_library(LIBRARY_OPENGL OpenGL)
    find_library(LIBRARY_IOKIT IOKit)
    set(LIBRARIES ${CMAKE_SOURCE_DIR}/lib/macos-clang/libglfw3.a ${LIBRARY_COCOA} ${LIBRARY_OPENGL} ${LIBRARY_IOKIT})
else()
    set(LIBRARIES GL)
endif()

add_executable(PolarVortex src/main.cpp ${SOURCES})
target_link_libraries(PolarVortex ${LIBRARIES})

add_executable(tests src/tests/tests.cpp ${SOURCES})
target_link_libraries(tests ${LIBRARIES})

file(COPY assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})