#include <iostream>

#include "assets/Asset.h"
#include "assets/AssetManager.h"
#include "memory/Memory.h"
#include "memory/StackAllocator.h"
#include "memory/PoolAllocator.h"
#include "game/Window.h"

int main() {

    // Load assets
    Assets = (AssetManager*)RootAllocator.Allocate(sizeof(AssetManager));
    new (Assets) AssetManager();

    // Create window
    MainWindow = (Window*)RootAllocator.Allocate(sizeof(Window));
    new (MainWindow) Window();

    PrintMemoryStats();

    // Run game
    MainWindow->Run();

    // Destroy window
    MainWindow->~Window();
    MainWindow = nullptr;

    return 0;
}