#pragma once

#include "Sprite.h"

/**
 * @brief A sprite that is displayed for a certain duration in an animation.
 */
struct SpriteFrame {
    /**
     * @brief The length of time in seconds seconds to display this frame before moving on to the next one.
     */
    float Duration;

    /**
     * @brief The sprite that is displayed while this frame is active.
     */
    Sprite Sprite;
};