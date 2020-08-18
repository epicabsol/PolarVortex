#pragma once

/**
 * @brief The possible ways to align an element along an axis into a destination rectangle.
 */
enum class UIAlignment {
    /**
     * @brief The element is aligned to the near edge of the destination rectangle, which is the top or left edge (assuming no bidirectional support).
     */
    Near,

    /**
     * @brief The element is aligned to the far edge of the destination rectangle, which is the bottom or right edge (assuming no bidirectional support).
     */
    Far,

    /**
     * @brief The element is centered within the destination rectangle along the axis.
     */
    Center,

    /**
     * @brief The element is stretched from the near edge of the destination rectangle to the far edge.
     */
    Stretch,
};