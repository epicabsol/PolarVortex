#include "world/Collision.h"

bool PointIntersectsBoundingBox(const Vector2& point, const BoundingBox& bounds, Intersection& intersection) {
    // Adapted from https://noonat.github.io/intersect/

    float dx = point.X - bounds.Position.X;
    float px = bounds.HalfSize.X - HMM_ABS(dx);
    if (px <= 0) {
        return false;
    }

    float dy = point.Y - bounds.Position.Y;
    float py = bounds.HalfSize.Y - HMM_ABS(dy);
    if (py <= 0) {
        return false;
    }

    if (px < py) {
        float xSign = HMM_SIGN(dx);
        intersection.Overlap = Vector2(px * xSign, 0.0f);
        intersection.Normal = Vector2(xSign, 0.0f);
        intersection.ContactPoint = Vector2(bounds.Position.X + bounds.HalfSize.X * xSign, point.Y);
    }
    else {
        float ySign = HMM_SIGN(dy);
        intersection.Overlap = Vector2(0.0f, py * ySign);
        intersection.Normal = Vector2(0.0f, ySign);
        intersection.ContactPoint = Vector2(point.X, bounds.Position.Y + bounds.HalfSize.Y * ySign);
    }
    intersection.EndPosition = intersection.ContactPoint;
    intersection.Time = 0.0f;
    return true;
}

bool LineIntersectsBoundingBox(const Vector2& point1, const Vector2& point2, const BoundingBox& bounds, Intersection& intersection) {
    Vector2 delta = point2 - point1;
    float scaleX = 1.0f / delta.X;
    float scaleY = 1.0f / delta.Y;
    float signX = HMM_SIGN(scaleX);
    float signY = HMM_SIGN(scaleY);
    float nearTimeX = (bounds.Position.X - signX * bounds.HalfSize.X - point1.X) * scaleX;
    float nearTimeY = (bounds.Position.Y - signY * bounds.HalfSize.Y - point1.Y) * scaleY;
    float farTimeX = (bounds.Position.X + signX * bounds.HalfSize.X - point1.X) * scaleX;
    float farTimeY = (bounds.Position.Y + signY * bounds.HalfSize.Y - point1.Y) * scaleY;

    if (nearTimeX > farTimeY || nearTimeY > farTimeX) {
        return false;
    }

    float nearTime = HMM_MAX(nearTimeX, nearTimeY);
    float farTime = HMM_MIN(farTimeX, farTimeY);

    intersection.Time = nearTime;
    if (nearTimeX > nearTimeY) {
        intersection.Normal = Vector2(-signX, 0.0f);
    }
    else {
        intersection.Normal = Vector2(0.0f, -signY);
    }

    intersection.Overlap = Vector2((1.0f - intersection.Time) * -delta.X, (1.0f - intersection.Time) * -delta.Y);
    intersection.ContactPoint = point1 + delta * intersection.Time;
    intersection.EndPosition = intersection.ContactPoint;

    return true;
}

bool BoundingBoxIntersectsBoundingBox(const BoundingBox& bounds1, const BoundingBox& bounds2, Intersection& intersection) {
    float dx = bounds2.Position.X - bounds1.Position.X;
    float px = (bounds2.HalfSize.X + bounds1.HalfSize.X) - HMM_ABS(dx);
    if (px <= 0) {
        return false;
    }

    float dy = bounds2.Position.Y - bounds1.Position.Y;
    float py = (bounds2.HalfSize.Y + bounds1.HalfSize.Y) - HMM_ABS(dy);
    if (py <= 0) {
        return false;
    }

    if (px < py) {
        float signX = HMM_SIGN(dx);
        intersection.Overlap = Vector2(px * signX, 0.0f);
        intersection.Normal = Vector2(signX, 0.0f);
        intersection.ContactPoint = Vector2(bounds1.Position.X + bounds1.HalfSize.X * signX, bounds2.Position.Y);
    }
    else {
        float signY = HMM_SIGN(dy);
        intersection.Overlap = Vector2(0.0f, py * signY);
        intersection.Normal = Vector2(0.0f, signY);
        intersection.ContactPoint = Vector2(bounds2.Position.X, bounds1.Position.Y + bounds1.HalfSize.Y * signY);
    }
    intersection.Time = 0.0f;
    intersection.EndPosition = bounds1.Position;
    return true;
}

bool SweptBoundingBoxIntersectsBoundingBox(const BoundingBox& bounds1, const Vector2& end, const BoundingBox& bounds2, Intersection& intersection) {
    // If the sweep segment starts and ends at the same point, it's just a simple AABB vs AABB check
    const Vector2& start = bounds1.Position;
    if (start == end) {
        if (BoundingBoxIntersectsBoundingBox(bounds1, bounds2, intersection)) {
            intersection.Time = 0.0f;
            return true;
        }
        else {
            intersection.Time = 1.0f;
            return false;
        }
    }

    // Minkowski says we can inflate the second AABB and check it against the sweep segment
    BoundingBox inflated(bounds2.Position, bounds2.HalfSize + bounds1.HalfSize);
    if (LineIntersectsBoundingBox(start, end, inflated, intersection) && intersection.Time >= 0.0f && intersection.Time <= 1.0f) {
        intersection.EndPosition = start + (end - start) * intersection.Time; // Lerp
        Vector2 direction = Math_NormalizeVec2(end - start);
        intersection.Overlap = Vector2(); // Doesn't need to move to be brought out of penetration
        intersection.ContactPoint = Vector2(HMM_CLAMP(intersection.EndPosition.X + direction.X * bounds1.HalfSize.X, bounds2.Position.X - bounds2.HalfSize.X, bounds2.Position.X + bounds2.HalfSize.X),
                                            HMM_CLAMP(intersection.EndPosition.Y + direction.Y * bounds1.HalfSize.Y, bounds2.Position.Y - bounds2.HalfSize.Y, bounds2.Position.Y + bounds2.HalfSize.Y));
        return true;
    }
    else {
        intersection.EndPosition = end;
        intersection.Time = 1.0f;
        return false;
    }
}

bool SegmentIntersectsWorld(const Vector2& start, const Vector2& end, const World* world, Intersection& intersection) {

    int cellX = (int)Math_Floor

    return false;
}

bool SweptBoundingBoxIntersectsWorld(const BoundingBox& bounds, const Vector2& end, const World* world, Intersection& intersection) {
    // TODO: Implement!
    return false;
}