#include "game/PolarVortexGame.h"

#include "assets/Asset.h"
#include "assets/AssetManager.h"
#include "game/Window.h"
#include "memory/Memory.h"
#include "memory/StackAllocator.h"
#include "memory/PoolAllocator.h"
#include "render/GLRenderer.h"
#include "screens/MainMenuScreen.h"
#include "screens/WorldScreen.h"

PolarVortexGame* Game;

PolarVortexGame::PolarVortexGame() : _AssetManager(nullptr), _MainWindow(nullptr), _Renderer(nullptr) {

}

PolarVortexGame::~PolarVortexGame() {
    // Destroy last screen
    if (this->_CurrentScreen != nullptr) {
        ScreenAllocator.Delete(this->_CurrentScreen);
    }
}

void PolarVortexGame::Load() {
    this->_AssetManager = RootAllocator.New<AssetManager>();
    this->_MainWindow = RootAllocator.New<Window>("Polar Vortex");
    this->_Renderer = RootAllocator.New<GLRenderer>();

    // Create main menu
    //MainMenuScreen* mainMenu = ScreenAllocator.New<MainMenuScreen>();
    //this->_CurrentScreen = mainMenu;

    // Create game screen
    this->_CurrentScreen = ScreenAllocator.New<WorldScreen>();

    PrintMemoryStats();
}

void PolarVortexGame::Run() {
    this->_LastUpdateTimestamp = std::chrono::high_resolution_clock::now();

    // Run window loop
    this->_MainWindow->Run();
}

void PolarVortexGame::Tick() {
    // TODO: Get input

    if (this->_CurrentScreen != nullptr) {
        // Update
        std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
        float timestep = std::chrono::duration_cast<std::chrono::duration<float>>(now - this->_LastUpdateTimestamp).count();
        this->_LastUpdateTimestamp = now;
        this->_CurrentScreen->Update(timestep);

        // Render
        this->_CurrentScreen->Render();
    }
}