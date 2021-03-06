#include "render/SpriteAnimation.h"

#include "assets/Asset.h"
#include "assets/AssetManager.h"
#include "assets/sajson.h"
#include "game/PolarVortexGame.h"
#include "memory/Memory.h"
#include "render/GLTexture.h"
#include "render/SpriteFrame.h"

#define JSON_BUFFER_LENGTH (1024 * 32)

SpriteAnimation::SpriteAnimation(Allocator& allocator, const char* data, size_t dataLength) : _Allocator(allocator), _Frames(nullptr), _FrameCount(0) {
    void* parseBuffer = ThreadAllocator.Allocate(JSON_BUFFER_LENGTH);

    const sajson::document doc = sajson::parse<sajson::single_allocation, sajson::mutable_string_view>(sajson::single_allocation((size_t*)parseBuffer, JSON_BUFFER_LENGTH), sajson::mutable_string_view(dataLength, (char*)data));

    sajson::value rootObject = doc.get_root();

    sajson::value framesArray = rootObject.get_value_of_key(sajson::literal("frames"));
    this->_FrameCount = framesArray.get_length();

    this->_Frames = (SpriteFrame*)allocator.Allocate(sizeof(SpriteFrame) * this->_FrameCount);

    for (size_t frameIndex = 0; frameIndex < this->_FrameCount; frameIndex++) {
        sajson::value frameObject = framesArray.get_array_element(frameIndex);
        sajson::value texturePath = frameObject.get_value_of_key(sajson::literal("texture"));
        Hash texturePathHash = HashData(texturePath.as_cstring(), texturePath.get_string_length());

        const GLTexture* texture = Game->GetAssetManager().GetAsset(texturePathHash)->GetAsset<GLTexture>();

        float duration = frameObject.get_value_of_key(sajson::literal("duration")).get_number_value();
        float x = frameObject.get_value_of_key(sajson::literal("x")).get_number_value();
        float y = frameObject.get_value_of_key(sajson::literal("y")).get_number_value();
        float width = frameObject.get_value_of_key(sajson::literal("width")).get_number_value();
        float height = frameObject.get_value_of_key(sajson::literal("height")).get_number_value();

        this->_Frames[frameIndex].Duration = duration;
        this->_Frames[frameIndex].Sprite.Texture = texture;
        this->_Frames[frameIndex].Sprite.UMin = x / texture->GetWidth();
        this->_Frames[frameIndex].Sprite.VMin = y / texture->GetHeight();
        this->_Frames[frameIndex].Sprite.USize = width / texture->GetWidth();
        this->_Frames[frameIndex].Sprite.VSize = height / texture->GetHeight();
    }

    ThreadAllocator.Free(parseBuffer);
}

SpriteAnimation::~SpriteAnimation() {
    this->_Allocator.Free(this->_Frames);
}