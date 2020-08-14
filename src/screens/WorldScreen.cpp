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

    float x = this->_Player->GetBounds().Position.X;
    float y = this->_Player->GetBounds().Position.Y + 1.0f;
    float scale = 0.5f;
    InputDevice* device = Game->GetMainWindow().GetInputDevice(0);
    Game->GetRenderer().DrawSprite(device->GetSprite(), x, y, 0.0f, scale, scale);
    x += scale * 1.25f;
    for (size_t i = 0; i < device->GetInputElementCount(); i++) {
        const InputElement* element = device->GetInputElement(i);
        if (element->GetValue() > 0.2f) {
            Game->GetRenderer().DrawSprite(element->GetSprite(), x, y, 0.0f, element->GetValue() * scale, element->GetValue() * scale);
            x += scale * 1.25f;
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
    InputDevice* device = Game->GetMainWindow().GetInputDevice(0);
    if (device->GetInputElement(0)->GetValue() > 0.5f) {
        this->_Player->GetVelocity().Y = 2.0f;
    }
    float speed = 3.0f;
    float xvelocity = 0.0f;
    if (device->GetInputElement(15)->GetValue() > 0.2f) {
        xvelocity -= device->GetInputElement(15)->GetValue() * speed;
    }
    if (device->GetInputElement(16)->GetValue() > 0.2f) {
        xvelocity += device->GetInputElement(16)->GetValue() * speed;
    }
    this->_Player->GetVelocity().X = xvelocity;
    this->_World->Update(timestep);
}

void WorldScreen::Resize(size_t width, size_t height) {
    this->_Viewports[0].SetLayout(0, 0, (int)width, (int)height);
}