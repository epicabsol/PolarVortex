#include "screens/WorldScreen.h"

#include "game/PolarVortexGame.h"
#include "game/Window.h"
#include "input/InputDevice.h"
#include "input/InputElement.h"
#include "memory/Memory.h"
#include "world/World.h"

void WorldScreen::RenderViewportContents(size_t index) {
    Screen::RenderViewportContents(index);
    this->_World->Render(this->_Viewports[index].GetCamera());

    float x = 0.25f;
    InputDevice* device = Game->GetMainWindow().GetInputDevice(0);
    Game->GetRenderer().DrawSprite(device->GetSprite(), x, 0.25f, 0.0f, 1.0f, 1.0f);
    x += 1.25f;
    for (size_t i = 0; i < device->GetInputElementCount(); i++) {
        const InputElement* element = device->GetInputElement(i);
        if (element->GetValue() > 0.2f) {
            Game->GetRenderer().DrawSprite(element->GetSprite(), x, 0.25f, 0.0f, element->GetValue(), element->GetValue());
            x += 1.25f;
        }
    }
}

WorldScreen::WorldScreen() : _World(nullptr), _MainCamera(0.0f, 0.0f, 10.0f, 10.0f), _Player(nullptr) {
    this->_World = ScreenAllocator.New<World>();
    this->_Player = this->_World->AddDynamicCollider(Vector2(0.0f, 3.0f), Vector2(0.5f, 0.5f), 1.0f);

    this->_Viewports[0].SetCamera(&this->_MainCamera);
    this->_Viewports[0].SetLayout(0, 0, (int)Game->GetMainWindow().GetClientWidth(), (int)Game->GetMainWindow().GetClientHeight());
}

WorldScreen::~WorldScreen() {
    ScreenAllocator.Delete(this->_World);
}

void WorldScreen::Update(float timestep) {
    if (Game->GetMainWindow().GetInputDevice(0)->GetInputElement(0)->GetValue() > 0.5f) {
        this->_Player->GetVelocity().Y = 2.0f;
    }
    this->_World->Update(timestep);
}

void WorldScreen::Resize(size_t width, size_t height) {
    this->_Viewports[0].SetLayout(0, 0, (int)width, (int)height);
}