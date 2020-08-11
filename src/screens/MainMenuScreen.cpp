#include "screens/MainMenuScreen.h"

#include "game/Camera.h"
#include "game/Window.h"
#include "memory/Memory.h"
#include "render/GLRenderer.h"
#include "render/GLTexture.h"
#include "game/PolarVortexGame.h"

void MainMenuScreen::RenderViewportContents(size_t index) {
    Screen::RenderViewportContents(index);
    Game->GetRenderer().DrawSprite(this->_TestTexture, 0, 0, 0, 1, 1);
    Game->GetRenderer().DrawSprite(this->_TestTexture, 0, 5, 0, 1, 1);
    Game->GetRenderer().DrawSprite(this->_TestTexture, 5, 0, 0, 1, 1);
}

MainMenuScreen::MainMenuScreen() {
    this->_TestTexture = ScreenAllocator.New<GLTexture>(STRINGHASH("assets/sprites/tile_dirt.png"));

    this->_MainCamera = ScreenAllocator.New<Camera>(0.0f, 0.0f, 0.0f, 10.0f);

    this->_Viewports[0].SetCamera(this->_MainCamera);
    this->_Viewports[0].SetLayout(0, 0, (int)Game->GetMainWindow().GetClientWidth(), (int)Game->GetMainWindow().GetClientHeight());
}

MainMenuScreen::~MainMenuScreen() {
    ScreenAllocator.Delete(this->_TestTexture);
    ScreenAllocator.Delete(this->_MainCamera);
}

void MainMenuScreen::Update(float timestep) {

}

void MainMenuScreen::Resize(size_t width, size_t height) {
    this->_Viewports[0].SetLayout(0, 0, (int)width, (int)height);
}