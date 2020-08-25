#pragma once

#include "Sprite.h"
#include "math.h"

class Allocator;
class GLTexture;

struct SpriteFontGlyph {
    /**
     * @brief The region of a texture and to draw for this glyph.
     */
    Sprite Sprite;

    /**
     * @brief The vertical distance from the baseline to where the sprite should be drawn.
     */
    float Top;

    /**
     * @brief The horizontal distance from the character's starting location to where the sprite should be drawn.
     */
    float Left;

    /**
     * @brief The distance to move horizontally after drawing this glyph.
     */
    float Advance;
};

class SpriteFont {
private:
    Allocator& _Allocator;
    float _LineHeight;
    float _Leading;
    float _Descent;
    SpriteFontGlyph* _Glyphs;
    size_t _GlyphCount;
    SpriteFontGlyph* _GlyphMap[0xFF];

public:
    SpriteFont(Allocator& allocator, const char* data, size_t dataLength);
    ~SpriteFont();

    /**
     * @brief Gets the distance to move the baseline down by when a new line is started.
     *
     * @return The line height of this font.
     */
    inline float GetLineHeight() const { return this->_LineHeight; }

    /**
     * @brief Gets the distance from the top of the caps height to the bottom of the previous line's descent.
     *
     * @return The leading distance of this font.
     */
    inline float GetLeading() const { return this->_Leading; }

    /**
     * @brief Gets the distance below the baseline that is reserved for glyphs which hang below the baseline.
     *
     * @return The descent distance of this font.
     */
    inline float GetDescent() const { return this->_Descent; }

    inline const SpriteFontGlyph* GetGlyphs() const { return this->_Glyphs; }
    inline size_t GetGlyphCount() const { return this->_GlyphCount; }
    inline const SpriteFontGlyph* GetGlyph(size_t index) const { return &this->_Glyphs[index]; }
    inline const SpriteFontGlyph* GetGlyph(char character) const { return this->_GlyphMap[character]; }

    Vector2 MeasureString(const char* string, float scale, float maxWidth) const;
};