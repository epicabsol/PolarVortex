#include <ui/UIButton.h>

#include <assets/Asset.h>
#include <assets/AssetManager.h>
#include <game/PolarVortexGame.h>
#include <render/GLRenderer.h>
#include <render/GLTexture.h>

Vector2 UIButton::MeasureContent() const {
    return UITextBlock::MeasureContent() + Vector2(this->_Padding.Left + this->_Padding.Right, this->_Padding.Top + this->_Padding.Bottom);
}

UIButton::UIButton(Allocator& allocator, const SpriteFont* font) : UITextBlock(allocator, font), _Padding(UIMargins(8.0f, 3.0f, 8.0f, 5.0f)), _NormalSprite(Game->GetAssetManager().GetAsset(STRINGHASH("assets/sprites/ui/parts.png"))->GetAsset<GLTexture>(), 0, 0, 16, 16) {

}

void UIButton::Render(const UIRectangle& absoluteParentBounds) {
    Game->GetRenderer().DrawNineSlice(this->_NormalSprite, absoluteParentBounds.Position.X + this->_Bounds.Position.X + this->_Bounds.Size.X * 0.5f, absoluteParentBounds.Position.Y + this->_Bounds.Position.Y + this->_Bounds.Size.Y * 0.5f, 0.0f, this->_Bounds.Size.X, this->_Bounds.Size.Y, Vector4(3.0f, 3.0f, 3.0f, 3.0f));

    UIRectangle shiftedBounds = absoluteParentBounds;
    shiftedBounds.Position.X += this->_Padding.Left;
    shiftedBounds.Position.Y -= this->_Padding.Top;
    UITextBlock::Render(shiftedBounds);
}