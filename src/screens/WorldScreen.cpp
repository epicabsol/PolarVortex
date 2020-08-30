#include "screens/WorldScreen.h"

#include "game/PolarVortexGame.h"
#include "game/Window.h"
#include "input/InputDevice.h"
#include "input/InputElement.h"
#include "memory/Memory.h"
#include "ui/UIAnimation.h"
#include "ui/UIDockContainer.h"
#include "ui/UISprite.h"
#include "ui/UITextBlock.h"
#include "world/Player.h"
#include "world/World.h"

void WorldScreen::RenderViewportContents(size_t index) {
    Screen::RenderViewportContents(index);

    Camera* camera = this->_Viewports[index].GetCamera();
    Game->GetRenderer().DrawSprite(this->_GridTexture, camera->GetPosition().X - camera->GetSize().X * 0.5f + 0.5f / this->_GridTexture->GetWidth(), -camera->GetPosition().Y - camera->GetSize().Y - 0.5f / this->_GridTexture->GetHeight(), camera->GetSize().X, camera->GetSize().Y, camera->GetPosition().X, camera->GetPosition().Y, 0.0f, camera->GetSize().X, camera->GetSize().Y);

    this->_World->Render(this->_Viewports[index].GetCamera());

    Game->GetRenderer().DrawString(Game->GetAssetManager().GetAsset(STRINGHASH("assets/fonts/vortex-body.pvf"))->GetAsset<SpriteFont>(), "CPT. Skytear", this->_Player->GetCollider()->GetBounds().Position.X - 0.65f, this->_Player->GetCollider()->GetBounds().Position.Y + 0.75f, 0.0f, 1.0f / 64.0f, 0.0f);

    float x = this->_Player->GetCollider()->GetBounds().Position.X;
    float y = this->_Player->GetCollider()->GetBounds().Position.Y + 1.0f;
    float scale = 0.25f;
    InputDevice* device = this->_Player->GetInputDevice();
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

WorldScreen::WorldScreen(Allocator& allocator) : Screen(allocator), _World(allocator.New<World>()), _MainCamera(allocator, 0.0f, 0.0f, 5.0f, 5.0f), _Player(nullptr), _HUDContainer(nullptr), _LeftContainer(nullptr), _RightContainer(nullptr), _WeaponSprite(nullptr), _GridTexture(Game->GetAssetManager().GetAsset(STRINGHASH("assets/sprites/grid.png"))->GetAsset<GLTexture>()) {
    this->_Player = allocator.New<Player>(Game->GetMainWindow().GetInputDevice(0));
    this->_World->AddObject(this->_Player);

    const GLTexture* gunTexture = Game->GetAssetManager().GetAsset(STRINGHASH("assets/sprites/weapons/pistol/icon_2x.png"))->GetAsset<GLTexture>();
    this->_WeaponSprite = allocator.New<UISprite>(Sprite(gunTexture));
    this->_RightContainer = allocator.New<UIDockContainer>(10);
    this->_LeftContainer = allocator.New<UIDockContainer>(10);
    this->_HUDContainer = allocator.New<UIDockContainer>(5);

    this->_RightContainer->AddElement(this->_WeaponSprite, DockSide::Bottom);

    const SpriteFont* font = Game->GetAssetManager().GetAsset(STRINGHASH("assets/fonts/vortex-body.pvf"))->GetAsset<SpriteFont>();
    UITextBlock* weaponName = allocator.New<UITextBlock>(font);
    weaponName->SetText("Reflex HC2");
    weaponName->SetHorizontalAlignment(UIAlignment::Far);
    weaponName->SetMargins(UIMargins(0.0f, 2.0f, 3.0f, -4.0f));
    this->_RightContainer->AddElement(weaponName, DockSide::Bottom);

    UISprite* playerHead = allocator.New<UISprite>(Sprite(Game->GetAssetManager().GetAsset(STRINGHASH("assets/sprites/icon_helmet.png"))->GetAsset<GLTexture>(), 0, 48, 48, 48));
    playerHead->SetMargins(UIMargins(3.0f, 0.0f, 0.0f, 12.0f));
    this->_LeftContainer->AddElement(playerHead, DockSide::Bottom);
    UITextBlock* playerName = allocator.New<UITextBlock>(font);
    playerName->SetText("CPT. Skytear");
    playerName->SetMargins(UIMargins(3.0f, 2.0f, 0.0f, 0.0f));
    this->_LeftContainer->AddElement(playerName, DockSide::Bottom);

    this->_HUDContainer->AddElement(this->_RightContainer, DockSide::Right);
    this->_HUDContainer->AddElement(this->_LeftContainer, DockSide::Left);

    this->_Viewports[0].SetCamera(&this->_MainCamera);
    this->_Viewports[0].SetLayout(0, 0, (int)Game->GetMainWindow().GetClientWidth(), (int)Game->GetMainWindow().GetClientHeight());
    this->_Viewports[0].SetRootUIElement(this->_HUDContainer);
}

WorldScreen::~WorldScreen() {
    this->_Allocator.Delete(this->_World);
}

void WorldScreen::Update(float timestep) {
    this->_World->Update(timestep);

    this->_MainCamera.SetPosition(Math_BlendExp(this->_MainCamera.GetPosition(), this->_Player->GetCollider()->GetBounds().Position, 0.05f, timestep));
}

void WorldScreen::Resize(size_t width, size_t height) {
    this->_Viewports[0].SetLayout(0, 0, (int)width, (int)height);
}