#pragma once

#include "../render/Math.h"
#include "BoundingBox.h"

class World;

struct Intersection {
    /**
     * @brief The point of contact on the second object.
     */
    Vector2 ContactPoint;

    /**
     * @brief The position where the ray or swept object intersected the second object.
     */
    Vector2 EndPosition;

    /**
     * @brief The shortest change in position that would take the first object out of the second object.
     */
    Vector2 Overlap;

    /**
     * @brief The unit direction perpendicular to the point of contact on the second object.
     */
    Vector2 Normal;

    /**
     * @brief The progress along the ray or sweep when the intersection occurred.
     */
    float Time;
};

bool PointIntersectsBoundingBox(const Vector2& point, const BoundingBox& bounds, Intersection& intersection);
bool LineIntersectsBoundingBox(const Vector2& point1, const Vector2& point2, const BoundingBox& bounds, Intersection& intersection);
bool BoundingBoxIntersectsBoundingBox(const BoundingBox& bounds1, const BoundingBox& bounds2, Intersection& intersection);
bool SweptBoundingBoxIntersectsBoundingBox(const BoundingBox& bounds1, const Vector2& end, const BoundingBox& bounds2, Intersection& intersection);

bool SegmentIntersectsWorld(const Vector2& start, const Vector2& end, const World* world, Intersection& intersection);
bool SweptBoundingBoxIntersectsWorld(const BoundingBox& bounds, const Vector2& end, const World* world, Intersection& intersection);