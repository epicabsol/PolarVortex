#include <iostream>

#include "assets/Asset.h"
#include "assets/AssetManager.h"
#include "game/Window.h"
#include "memory/Memory.h"
#include "memory/StackAllocator.h"
#include "memory/PoolAllocator.h"
#include "render/GLRenderer.h"
#include "screens/MainMenuScreen.h"

int main() {

    // Load assets
    Assets = RootAllocator.New<AssetManager>();

    // Create window
    MainWindow = RootAllocator.New<Window>("Polar Vortex");

    // Create renderer
    Renderer = RootAllocator.New<GLRenderer>();

    // Create main menu
    MainMenuScreen* mainMenu = ScreenAllocator.New<MainMenuScreen>();
    CurrentScreen = mainMenu;

    PrintMemoryStats();

    // Run game
    MainWindow->Run();

    // Destroy last screen
    if (CurrentScreen != nullptr) {
        ScreenAllocator.Delete(CurrentScreen);
    }

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