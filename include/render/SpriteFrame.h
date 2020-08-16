#pragma once

/**
 * @brief Specifies a frame of a sprite animation as a rectangular region of some source texture that is displayed for a certain time duration.
 */
struct SpriteFrame {
    /**
     * @brief The length of time in seconds seconds to display this frame before moving on to the next one.
     */
    float Duration;

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
     * 
     */
    float VSize;
};