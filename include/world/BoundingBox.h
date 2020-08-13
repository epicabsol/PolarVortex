#pragma once

#include "../render/Math.h"

/**
 * @brief 2D axis-aligned bounding box.
 */
struct BoundingBox {
    /**
     * @brief The location of the center of this bounding box.
     */
    Vector2 Position;

    /**
     * @brief Half of the width and height of this bounding box.
     */
    Vector2 HalfSize;

    /**
     * @brief Constructs a new Bounding Box object with the given position and half-size.
     *
     * @param position The location of the center of the bounding box.
     * @param halfSize Half of the size of the bounding box.
     */
    inline BoundingBox(Vector2 position, Vector2 halfSize) : Position(position), HalfSize(halfSize) { }

    /**
     * @brief Calculates the full size of this bounding box.
     *
     * @return The size of this bounding box.
     */
    inline Vector2 GetSize() const { return 2.0f * this->HalfSize; }

    /**
     * @brief Calculates the position of the minimum corner (the corner with the lower X and Y coordinates).
     *
     * @return The position of the minimum corner.
     */
    inline Vector2 GetMin() const { return this->Position - this->HalfSize; }

    /**
     * @brief Calculates the position of the maximum corner (the corner with the higher X and Y coordinates).
     *
     * @return The position of the maximum corner.
     */
    inline Vector2 GetMax() const { return this->Position + this->HalfSize; }
};