#include <iostream>

#include "assets/Asset.h"
#include "assets/AssetManager.h"
#include "game/Window.h"
#include "memory/Memory.h"
#include "memory/StackAllocator.h"
#include "memory/PoolAllocator.h"
#include "render/GLRenderer.h"

int main() {

    // Load assets
    Assets = RootAllocator.New<AssetManager>();

    // Create window
    MainWindow = RootAllocator.New<Window>("Polar Vortex");

    // Create renderer
    Renderer = RootAllocator.New<GLRenderer>();

    PrintMemoryStats();

    // Run game
    MainWindow->Run();

    // Destroy renderer
    RootAllocator.Delete(Renderer);
    Renderer = nullptr;

    // Destroy window
    RootAllocator.Delete(MainWindow);
    MainWindow = nullptr;

    RootAllocator.Delete(Assets);
    Assets = nullptr;

    return 0;
}