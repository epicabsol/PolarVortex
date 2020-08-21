#include "screens/WorldScreen.h"

#include "game/PolarVortexGame.h"
#include "game/Window.h"
#include "input/InputDevice.h"
#include "input/InputElement.h"
#include "memory/Memory.h"
#include "ui/UIAnimation.h"
#include "ui/UIDockContainer.h"
#include "ui/UISprite.h"
#include "world/World.h"

void WorldScreen::RenderViewportContents(size_t index) {
    Screen::RenderViewportContents(index);
    this->_World->Render(this->_Viewports[index].GetCamera());

    Game->GetRenderer().DrawSprite(this->_PlayerAnimation, this->_Player->GetBounds().Position.X, this->_Player->GetBounds().Position.Y, 0.0f, 1.0f, 1.0f);
    Game->GetRenderer().DrawString(Game->GetAssetManager().GetAsset(STRINGHASH("assets/fonts/vortex-body.pvf"))->GetAsset<SpriteFont>(), "Dirt Reincarnate", this->_Player->GetBounds().Position.X - 1.25f, this->_Player->GetBounds().Position.Y + 0.75f, 0.0f, 1.0f / 32.0f, 0.0f);

    float x = this->_Player->GetBounds().Position.X;
    float y = this->_Player->GetBounds().Position.Y + 1.0f;
    float scale = 0.5f;
    InputDevice* device = Game->GetMainWindow().GetInputDevice(0);
    Game->GetRenderer().DrawSprite(device->GetSprite(), x, y, 0.0f, scale, scale);
    Game->GetRenderer().DrawString(Game->GetAssetManager().GetAsset(STRINGHASH("assets/fonts/vortex-body.pvf"))->GetAsset<SpriteFont>(), device->GetName(), x - 0.25f, y + 0.5f, 0.0f, 1.0f / 64.0f, 0.0f);

    x += scale * 1.25f;
    for (size_t i = 0; i < device->GetInputElementCount(); i++) {
        const InputElement* element = device->GetInputElement(i);
        if (element->GetValue() > 0.2f) {
            Game->GetRenderer().DrawSprite(element->GetSprite(), x, y, 0.0f, element->GetValue() * scale, element->GetValue() * scale);
            x += scale * 1.25f;
        }
    }
}

WorldScreen::WorldScreen(Allocator& allocator) : Screen(allocator), _World(allocator.New<World>()), _MainCamera(allocator, 0.0f, 0.0f, 5.0f, 5.0f), _Player(nullptr), _PlayerIdleAnimation(Game->GetAssetManager().GetAsset(STRINGHASH("assets/sprites/character/character_idle.pva"))->GetAsset<SpriteAnimation>()), _PlayerAnimation(allocator, this->_PlayerIdleAnimation), _HUDContainer(nullptr), _RightContainer(nullptr), _WeaponSprite(nullptr) {
    this->_Player = this->_World->AddDynamicCollider(Vector2(0.0f, 3.0f), Vector2(0.15f, 0.35f), 1.0f);

    const GLTexture* gunTexture = Game->GetAssetManager().GetAsset(STRINGHASH("assets/sprites/weapons/pistol/icon.png"))->GetAsset<GLTexture>();
    this->_WeaponSprite = allocator.New<UISprite>(Sprite(gunTexture));
    this->_RightContainer = allocator.New<UIDockContainer>(10);
    this->_HUDContainer = allocator.New<UIDockContainer>(5);

    this->_RightContainer->AddElement(this->_WeaponSprite, DockSide::Bottom);

    this->_HUDContainer->AddElement(this->_RightContainer, DockSide::Right);

    this->_Viewports[0].SetCamera(&this->_MainCamera);
    this->_Viewports[0].SetLayout(0, 0, (int)Game->GetMainWindow().GetClientWidth(), (int)Game->GetMainWindow().GetClientHeight());
    this->_Viewports[0].SetRootUIElement(this->_HUDContainer);
}

WorldScreen::~WorldScreen() {
    this->_Allocator.Delete(this->_World);
}

void WorldScreen::Update(float timestep) {
    InputDevice* device = Game->GetMainWindow().GetInputDevice(0);
    if (this->_Player->IsOnGround() && device->GetInputElement(0)->GetValue() > 0.5f) {
        this->_Player->GetVelocity().Y = 5.0f;
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
    this->_PlayerAnimation.Advance(timestep);

    this->_MainCamera.SetPosition(Math_BlendExp(this->_MainCamera.GetPosition(), this->_Player->GetBounds().Position, 0.05f, timestep));
}

void WorldScreen::Resize(size_t width, size_t height) {
    this->_Viewports[0].SetLayout(0, 0, (int)width, (int)height);
}