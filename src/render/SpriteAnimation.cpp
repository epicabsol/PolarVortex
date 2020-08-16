#include "render/SpriteAnimation.h"

#include "assets/Asset.h"
#include "assets/AssetManager.h"
#include "assets/sajson.h"
#include "game/PolarVortexGame.h"
#include "memory/Memory.h"
#include "render/GLTexture.h"
#include "render/SpriteFrame.h"

#define JSON_BUFFER_LENGTH (1024 * 32)

SpriteAnimation::SpriteAnimation(Allocator& allocator, Hash layoutPathHash) : _Allocator(allocator), _Frames(nullptr), _FrameCount(0) {
    Asset* asset = Game->GetAssetManager().GetAsset(layoutPathHash);

    void* parseBuffer = allocator.Allocate(JSON_BUFFER_LENGTH);
    
    const sajson::document doc = sajson::parse<sajson::single_allocation, sajson::mutable_string_view>(sajson::single_allocation((size_t*)parseBuffer, JSON_BUFFER_LENGTH), sajson::mutable_string_view(asset->GetDataLength(), (char*)asset->GetData()));

    sajson::value rootObject = doc.get_root();
    sajson::value texturePath = rootObject.get_value_of_key(sajson::literal("texture"));
    Hash texturePathHash = HashData(texturePath.as_cstring(), texturePath.get_string_length());

    this->_Texture = allocator.New<GLTexture>(texturePathHash);

    sajson::value framesArray = rootObject.get_value_of_key(sajson::literal("frames"));
    this->_FrameCount = framesArray.get_length();

    this->_Frames = (SpriteFrame*)allocator.Allocate(sizeof(SpriteFrame) * this->_FrameCount);

    for (size_t frameIndex = 0; frameIndex < this->_FrameCount; frameIndex++) {
        sajson::value frameObject = framesArray.get_array_element(frameIndex);

        float duration = frameObject.get_value_of_key(sajson::literal("duration")).get_number_value();
        float x = frameObject.get_value_of_key(sajson::literal("x")).get_number_value();
        float y = frameObject.get_value_of_key(sajson::literal("y")).get_number_value();
        float width = frameObject.get_value_of_key(sajson::literal("width")).get_number_value();
        float height = frameObject.get_value_of_key(sajson::literal("height")).get_number_value();

        this->_Frames[frameIndex].Duration = duration;
        this->_Frames[frameIndex].UMin = x / this->_Texture->GetWidth();
        this->_Frames[frameIndex].VMin = y / this->_Texture->GetHeight();
        this->_Frames[frameIndex].USize = width / this->_Texture->GetWidth();
        this->_Frames[frameIndex].VSize = height / this->_Texture->GetHeight();
    }

    FrameAllocator.Free(parseBuffer);
}

SpriteAnimation::~SpriteAnimation() {
    this->_Allocator.Delete(this->_Texture);
    this->_Allocator.Free(this->_Frames);
}