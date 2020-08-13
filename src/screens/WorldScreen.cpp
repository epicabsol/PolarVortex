#include "screens/WorldScreen.h"

#include "game/PolarVortexGame.h"
#include "memory/Memory.h"
#include "world/World.h"

void WorldScreen::RenderViewportContents(size_t index) {
    Screen::RenderViewportContents(index);
    this->_World->Render(this->_Viewports[index].GetCamera());
}

WorldScreen::WorldScreen() : _World(nullptr), _MainCamera(0.0f, 0.0f, 10.0f, 10.0f) {
    this->_World = ScreenAllocator.New<World>();

    this->_Viewports[0].SetCamera(&this->_MainCamera);
    this->_Viewports[0].SetLayout(0, 0, (int)Game->GetMainWindow().GetClientWidth(), (int)Game->GetMainWindow().GetClientHeight());
}

WorldScreen::~WorldScreen() {
    ScreenAllocator.Delete(this->_World);
}

void WorldScreen::Update(float timestep) {
    this->_World->Update(timestep);
}

void WorldScreen::Resize(size_t width, size_t height) {
    this->_Viewports[0].SetLayout(0, 0, (int)width, (int)height);
}