#pragma once

class GLTexture;

/**
 * @brief A region of a texture.
 */
struct Sprite {
    /**
     * @brief The texture with the pixel data to display.
     */
    GLTexture* Texture;

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
};