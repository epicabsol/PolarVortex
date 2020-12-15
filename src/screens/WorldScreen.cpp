#include "screens/WorldScreen.h"

#include "game/PolarVortexGame.h"
#include "game/Window.h"
#include "input/InputDevice.h"
#include "input/InputElement.h"
#include "memory/Memory.h"
#include "render/GLVertexAttribute.h"
#include "ui/UIAnimation.h"
#include "ui/UIDockContainer.h"
#include "ui/UISprite.h"
#include "ui/UITextBlock.h"
#include "world/TilePalette.h"
#include "world/Player.h"
#include "world/World.h"

static GLVertexAttribute SpriteVertexAttributes[] = {
    { 3, GL_FLOAT, sizeof(float) },
    { 2, GL_FLOAT, sizeof(float) },
};
static const size_t SpriteVertexAttributeCount = 2;

void WorldScreen::RenderViewportContents(size_t index) {
    Screen::RenderViewportContents(index);

    Camera* camera = this->_Viewports[index].GetCamera();

    // Draw grid background
    Game->GetRenderer().DrawSprite(this->_GridTexture, camera->GetPosition().X - camera->GetSize().X * 0.5f + 0.5f / this->_GridTexture->GetWidth(), -camera->GetPosition().Y - camera->GetSize().Y - 0.5f / this->_GridTexture->GetHeight(), camera->GetSize().X, camera->GetSize().Y, camera->GetPosition().X, camera->GetPosition().Y, 0.0f, camera->GetSize().X, camera->GetSize().Y);

    this->_World->Render(this->_Viewports[index].GetCamera());

    Game->GetRenderer().DrawString(Game->GetAssetManager().GetAsset(STRINGHASH("assets/fonts/vortex-body.pvf"))->GetAsset<SpriteFont>(), "CPT. Skytear", this->_Player->GetCollider()->GetBounds().Position.X - 2.35f, this->_Player->GetCollider()->GetBounds().Position.Y + 3.0f, 0.0f, 1.0f / 16.0f, 0.0f);

    float x = this->_Player->GetCollider()->GetBounds().Position.X;
    float y = this->_Player->GetCollider()->GetBounds().Position.Y + 4.0f;
    float scale = 1.0f;
    InputDevice* device = this->_Player->GetInputDevice();
    Game->GetRenderer().DrawSprite(device->GetSprite(), x, y, 0.0f, scale, scale);
    Game->GetRenderer().DrawString(Game->GetAssetManager().GetAsset(STRINGHASH("assets/fonts/vortex-body.pvf"))->GetAsset<SpriteFont>(), device->GetName(), x - 0.25f, y + 1.25f, 0.0f, 1.0f / 16.0f, 0.0f);

    x += scale * 1.25f;
    for (size_t i = 0; i < device->GetInputElementCount(); i++) {
        const InputElement* element = device->GetInputElement(i);
        if (element->GetValue() > 0.2f) {
            Game->GetRenderer().DrawSprite(element->GetSprite(), x, y, 0.0f, element->GetValue() * scale, element->GetValue() * scale);
            x += scale * 1.25f;
        }
    }


    glUseProgram(this->_TileShaderProgram->GetProgramHandle());
    Matrix transform = Math_Mat4d(1.0f);
    unsigned int transformUniform = glGetUniformLocation(this->_TileShaderProgram->GetProgramHandle(), "Transform");
    glUniformMatrix4fv(transformUniform, 1, GL_FALSE, &transform.Elements[0][0]);
    unsigned int projectionUniform = glGetUniformLocation(this->_TileShaderProgram->GetProgramHandle(), "Projection");
    glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &Game->GetRenderer().GetProjection().Elements[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->_World->GetTilePalette()->GetTexture()->GetHandle());
    Game->GetRenderer().DrawMesh(this->_TileMesh, this->_TileShaderProgram);
}

void WorldScreen::RebuildTileMesh() {
    if (this->_TileMesh != nullptr) {
        this->_Allocator.Delete(this->_TileMesh);
    }

    // Allocate the vertices and indices (allocates for worst-case, usually there will be some memory unused)
    SpriteVertex* vertices = (SpriteVertex*)this->_Allocator.Allocate(sizeof(SpriteVertex) * this->_World->GetWidth() * this->_World->GetHeight() * 4);
    size_t vertexCount = 0;
    uint16_t* indices = (uint16_t*)this->_Allocator.Allocate(sizeof(uint16_t) * this->_World->GetWidth() * this->_World->GetHeight() * 6);
    size_t indexCount = 0;

    // Generate the vertices and indices
    for (size_t y = 0; y < this->_World->GetHeight(); y++) {
        for (size_t x = 0; x < this->_World->GetWidth(); x++) {
            int16_t index = this->_World->GetTile(x, y).PaletteIndex;
            if (index >= 0) {
                float minU = (index % this->_World->GetTilePalette()->GetStride()) * this->_World->GetTilePalette()->GetTileWidth();
                float minV = (index / this->_World->GetTilePalette()->GetStride()) * this->_World->GetTilePalette()->GetTileHeight();
                vertices[vertexCount] = { (float)x, (float)y, 0.0f, minU, minV + this->_World->GetTilePalette()->GetTileHeight() }; // Bottom left
                vertices[vertexCount + 1] = { (float)x + 1.0f, (float)y, 0.0f, minU + this->_World->GetTilePalette()->GetTileWidth(), minV + this->_World->GetTilePalette()->GetTileHeight() }; // Bottom right
                vertices[vertexCount + 2] = { (float)x + 1.0f, (float)y + 1.0f, 0.0f, minU + this->_World->GetTilePalette()->GetTileWidth(), minV }; // Top right
                vertices[vertexCount + 3] = { (float)x, (float)y + 1.0f, 0.0f, minU, minV }; // Top left

                indices[indexCount] = vertexCount;
                indices[indexCount + 1] = vertexCount + 2;
                indices[indexCount + 2] = vertexCount + 1;
                indices[indexCount + 3] = vertexCount;
                indices[indexCount + 4] = vertexCount + 3;
                indices[indexCount + 5] = vertexCount + 2;

                vertexCount += 4;
                indexCount += 6;
            }
        }
    }

    this->_TileMesh = this->_Allocator.New<GLMesh>(SpriteVertexAttributes, SpriteVertexAttributeCount, vertices, vertexCount, indices, sizeof(indices[0]), indexCount);
    this->_Allocator.Free(vertices);
}

WorldScreen::WorldScreen(Allocator& allocator) : Screen(allocator), _World(allocator.New<World>(Game->GetAssetManager().GetAsset(STRINGHASH("assets/worlds/test.pvw"))->GetAsset<WorldBlueprint>())), _MainCamera(allocator, 0.0f, 0.0f, 20.0f, 20.0f), _Player(nullptr), _TileMesh(nullptr), _TileShaderProgram(nullptr), _HUDContainer(nullptr), _LeftContainer(nullptr), _RightContainer(nullptr), _WeaponSprite(nullptr), _GridTexture(Game->GetAssetManager().GetAsset(STRINGHASH("assets/sprites/grid.png"))->GetAsset<GLTexture>()) {
    this->_Player = allocator.New<Player>(Game->GetMainWindow().GetInputDevice(0));
    this->_World->AddObject(this->_Player);
    this->_Player->GetCollider()->GetBounds().Position = Vector2(this->_World->GetWidth() * 0.5f, this->_World->GetHeight());

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

    this->_TileShaderProgram = this->_Allocator.New<GLShaderProgram>(STRINGHASH("assets/shaders/TileVertexShader.glsl"), STRINGHASH("assets/shaders/SpritePixelShader.glsl"));
    this->RebuildTileMesh();
}

WorldScreen::~WorldScreen() {
    this->_Allocator.Delete(this->_TileShaderProgram);
    if (this->_TileMesh != nullptr) {
        this->_Allocator.Delete(this->_TileMesh);
    }
    this->_Allocator.Delete(this->_World);
}

void WorldScreen::Update(float timestep) {
    this->_World->Update(timestep);

    this->_MainCamera.SetPosition(Math_BlendExp(this->_MainCamera.GetPosition(), this->_Player->GetCollider()->GetBounds().Position, 0.05f, timestep));
}

void WorldScreen::Resize(size_t width, size_t height) {
    this->_Viewports[0].SetLayout(0, 0, (int)width, (int)height);
}