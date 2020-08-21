#include "render/SpriteFont.h"

#include "assets/AssetManager.h"
#include "assets/sajson.h"
#include "game/PolarVortexGame.h"
#include "memory/Allocator.h"

#define JSON_BUFFER_LENGTH (1024 * 32)

SpriteFont::SpriteFont(Allocator& allocator, const char* data, size_t dataLength) : _Allocator(allocator) {
    void* parseBuffer = allocator.Allocate(JSON_BUFFER_LENGTH);

    const sajson::document doc = sajson::parse<sajson::single_allocation, sajson::mutable_string_view>(sajson::single_allocation((size_t*)parseBuffer, JSON_BUFFER_LENGTH), sajson::mutable_string_view(dataLength, (char*)data));

    sajson::value rootObject = doc.get_root();
    sajson::value texturePath = rootObject.get_value_of_key(sajson::literal("texture"));
    Hash texturePathHash = HashData(texturePath.as_cstring(), texturePath.get_string_length());
    const GLTexture* texture = Game->GetAssetManager().GetAsset(texturePathHash)->GetAsset<GLTexture>();

    this->_LineHeight = rootObject.get_value_of_key(sajson::literal("line-height")).get_number_value();
    this->_Leading = rootObject.get_value_of_key(sajson::literal("leading")).get_number_value();
    this->_Descent = rootObject.get_value_of_key(sajson::literal("descent")).get_number_value();

    sajson::value glyphArray = rootObject.get_value_of_key(sajson::literal("glyphs"));
    this->_GlyphCount = glyphArray.get_length();
    this->_Glyphs = (SpriteFontGlyph*)allocator.Allocate(sizeof(SpriteFontGlyph) * this->_GlyphCount);
    for (size_t i = 0; i < sizeof(this->_GlyphMap) / sizeof(this->_GlyphMap[0]); i++) {
        this->_GlyphMap[i] = nullptr;
    }
    for (size_t i = 0; i < this->_GlyphCount; i++) {
        sajson::value glyphObject = glyphArray.get_array_element(i);
        char character = glyphObject.get_value_of_key(sajson::literal("character")).as_cstring()[0];
        this->_GlyphMap[character] = &this->_Glyphs[i];
        this->_Glyphs[i].Sprite.Texture = texture;
        this->_Glyphs[i].Sprite.UMin = glyphObject.get_value_of_key(sajson::literal("x")).get_number_value() / texture->GetWidth();
        this->_Glyphs[i].Sprite.VMin = glyphObject.get_value_of_key(sajson::literal("y")).get_number_value() / texture->GetHeight();
        this->_Glyphs[i].Sprite.USize = glyphObject.get_value_of_key(sajson::literal("width")).get_number_value() / texture->GetWidth();
        this->_Glyphs[i].Sprite.VSize = glyphObject.get_value_of_key(sajson::literal("height")).get_number_value() / texture->GetHeight();
        this->_Glyphs[i].Top = glyphObject.get_value_of_key(sajson::literal("top")).get_number_value();
        this->_Glyphs[i].Left = glyphObject.get_value_of_key(sajson::literal("left")).get_number_value();
        this->_Glyphs[i].Advance = glyphObject.get_value_of_key(sajson::literal("advance")).get_number_value();
    }

    allocator.Free(parseBuffer);
}

SpriteFont::~SpriteFont() {
    this->_Allocator.Free(this->_Glyphs);
}

Vector2 SpriteFont::MeasureString(const char* string, float scale, float maxWidth) const {
    Vector2 result(0.0f, this->_LineHeight * scale);

    size_t length = strlen(string);
    float x = 0.0f;
    for (size_t i = 0; i < length; i++) {
        const SpriteFontGlyph* glyph = this->_GlyphMap[string[i]];

        if (glyph == nullptr) {
            continue;
        }

        if (maxWidth > 0.0f && x + (glyph->Left + glyph->Sprite.USize * glyph->Sprite.Texture->GetWidth()) * scale > maxWidth) {
            result.X = HMM_MAX(x, result.X);
            x = 0.0f;
            result.Y += this->_LineHeight;
        }

        x += glyph->Advance * scale;
    }

    result.X = HMM_MAX(x, result.X);

    return result;
}