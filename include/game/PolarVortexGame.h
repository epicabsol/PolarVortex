#pragma once

#include <chrono>

#include "../assets/AssetManager.h"
#include "Window.h"
#include "render/GLRenderer.h"

class Screen;

class PolarVortexGame {
private:
    AssetManager* _AssetManager;
    Window* _MainWindow;
    GLRenderer* _Renderer;

    std::chrono::high_resolution_clock::time_point _LastUpdateTimestamp;

    Screen* _CurrentScreen;

public:
    PolarVortexGame();
    ~PolarVortexGame();

    inline AssetManager& GetAssetManager() { return *this->_AssetManager; }
    inline Window& GetMainWindow() { return *this->_MainWindow; }
    inline GLRenderer& GetRenderer() { return *this->_Renderer; }
    inline Screen* GetCurrentScreen() { return this->_CurrentScreen; }

    void Load();
    void Run();
    void Tick();
};

extern PolarVortexGame* Game;