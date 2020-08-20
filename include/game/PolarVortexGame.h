#pragma once

#include "Window.h"
#include "render/GLRenderer.h"

class AssetManager;
class Screen;

class PolarVortexGame {
private:
    AssetManager* _AssetManager;
    Window* _MainWindow;
    GLRenderer* _Renderer;

    uint64_t _LastUpdateTimestamp;

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