#pragma once

#include "GLTexture.h"

/**
 * @brief A region of a texture.
 */
struct Sprite {
    /**
     * @brief The texture with the pixel data to display.
     */
    const GLTexture* Texture;

    /**
     * @brief The U coordinate of the top-left corner of the frame.
     */
    float UMin;

    /**
     * @brief The V coordinate of the top-left corner of the frame.
     */
    float VMin;

    /**
     * @brief The horizontal width of this frame in the texture.
     */
    float USize;

    /**
     * @brief The vertical height of this frame in the texture.
     */
    float VSize;

    inline Sprite() : Texture(nullptr), UMin(0.0f), VMin(0.0f), USize(1.0f), VSize(1.0f) { }
    inline Sprite(const GLTexture* texture) : Texture(texture), UMin(0.0f), VMin(0.0f), USize(1.0f), VSize(1.0f) { }
    inline Sprite(const GLTexture* texture, int left, int top, int width, int height) : Texture(texture), UMin(left / (float)texture->GetWidth()), VMin(top / (float)texture->GetHeight()), USize(width / (float)texture->GetWidth()), VSize(height / (float)texture->GetHeight()) { }
};