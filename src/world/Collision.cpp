#include "world/Collision.h"

#include <math.h>
#include "world/Grid.h"
#include "world/World.h"

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
    // If we are starting inside the bounding box, then the line intersects.
    if (PointIntersectsBoundingBox(point1, bounds, intersection)) {
        intersection.Time = 0.0f;
        return true;
    }

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

bool SegmentIntersectsGrid(const Vector2& start, const Vector2& end, const Grid* grid, Intersection& intersection) {

    // If the segment doesn't intersect the grid bounds, then we can skip checking the grid tiles
    if (!LineIntersectsBoundingBox(start, end, BoundingBox(grid->GetPosition() + Vector2((float)grid->GetWidth(), (float)grid->GetHeight()) * 0.5f, Vector2((float)grid->GetWidth(), (float)grid->GetHeight()) * 0.5f), intersection)) {
        return false;
    }

    Vector2 startRelative = start - grid->GetPosition();
    Vector2 endRelative = end - grid->GetPosition();

    int cellX = (int)floorf(startRelative.X);
    int cellY = (int)floorf(startRelative.Y);

    // Test the starting cell
    if (cellX >= 0 && cellX < grid->GetWidth()
        && cellY >= 0 && cellY < grid->GetHeight()
        && grid->GetTile(cellX, cellY).Collides) {

        // We are colliding with the initial cell
        PointIntersectsBoundingBox(startRelative, BoundingBox(Vector2(cellX + 0.5f, cellY + 0.5f), Vector2(0.5f, 0.5f)), intersection);
        return true;
    }

    float x = startRelative.X;
    float y = startRelative.Y;
    int endCellX = (int)floorf(endRelative.X);
    int endCellY = (int)floorf(endRelative.Y);
    int xdir = (endRelative.X - startRelative.X >= 0) ? 1 : -1;
    int ydir = (endRelative.Y - startRelative.Y >= 0) ? 1 : -1;

    float dy; // The slope, or, how far in the y we move for each 1 unit of x
    if (endRelative.X != startRelative.X) {
        dy = (endRelative.Y - startRelative.Y) / (endRelative.X - startRelative.X);
    }
    else {
        dy = INFINITY;
    }

    float dx; // How far in the x we move for each 1 unit of y
    if (endRelative.Y != startRelative.Y) {
        dx = (endRelative.X - startRelative.X) / (endRelative.Y - startRelative.Y);
    }
    else {
        dx = INFINITY;
    }

    // Determine how far it will be until we hit the next cell in the X direction
    int nextX = xdir >= 0 ? (int)floorf(x + 1.0f) : (int)floorf(x);
    float stepX = nextX - x; // How far in X to the next X coordinate
    float toNextX = sqrtf(stepX * stepX + (stepX * dy) * (stepX * dy)); // How far along the line to the next X coordinate
    float xIncrement = sqrtf(1.0f * 1.0f + dy * dy);

    // Determine how far it will be until we hit the next cell in the Y direction
    int nextY = ydir >= 0 ? (int)floorf(y + 1.0f) : (int)floorf(y);
    float stepY = nextY - y; // How far in Y to the next Y coordinate
    float toNextY = sqrtf(stepY * stepY + (stepY * dx) * (stepY * dx)); // How far along the line to the next Y coordinate
    float yIncrement = sqrtf(dx * dx + 1.0f * 1.0f);

    Vector2 hitNormal = Vector2();

    // Keep stepping until we reach the destination cell
    while (cellX != endCellX || cellY != endCellY) {
        // Determine whether the next X value is closer, or the next Y value
        if (toNextX < toNextY) {
            // Move to the next X value
            cellX += xdir;
            x += stepX;
            y += stepX * dy;

            // Adjust our movement plan along the other axis
            stepY -= stepX * dy; // Convert how far we moved in X to how far we moved in Y
            toNextY -= toNextX;

            // Calculate for the new next X value
            nextX = cellX + xdir;
            stepX = 1.0f * xdir;
            toNextX = xIncrement;

            hitNormal = Vector2((float)-xdir, 0.0f);
        }
        else {
            // Move to the next Y value
            cellY += ydir;
            y += stepY;
            x += stepY * dx;

            // Adjust our movement plan along the other axis
            stepX -= stepY * dx;
            toNextX -= toNextY;

            // Calculate for the next next Y value
            nextY = cellY + ydir;
            stepY = 1.0f * ydir;
            toNextY = yIncrement;

            hitNormal = Vector2(0.0f, (float)-ydir);
        }

        // Check whether the cell we arrived at is in the grid
        if (cellX >= 0 && cellX < grid->GetWidth()
            && cellY >= 0 && cellY < grid->GetHeight()) {
            // Check whether it should end this test
            if (grid->GetTile(cellX, cellY).Collides) {
                // end position, overlap, contact point
                intersection.EndPosition = Vector2(x, y) + grid->GetPosition();
                intersection.Overlap = Vector2();
                intersection.ContactPoint = intersection.EndPosition;
                intersection.Normal = hitNormal;
                float total = sqrtf((endRelative.X - startRelative.X) * (endRelative.X - startRelative.X) + (endRelative.Y - startRelative.Y) * (endRelative.Y - startRelative.Y));
                float distance = sqrtf((x - startRelative.X) * (x - startRelative.X) + (y - startRelative.Y) * (y - startRelative.Y));
                intersection.Time = distance / total;
                return true;
            }
        }
        else if (xdir > 0 && cellX >= grid->GetWidth()
            || xdir < 0 && cellX < 0
            || ydir > 0 && cellY >= grid->GetHeight()
            || ydir < 0 && cellY < 0) {
            // The segment is off the grid and travelling away from the grid
            // so it will never collide.
            return false;
        }
    }

    // If we reach here, no cells were hit
    return false;
}

bool SegmentIntersectsWorld(const Vector2& start, const Vector2& end, const World* world, Intersection& intersection) {
    float nearestTime = 100.0f; // Anything larger than 1.0 works
    bool didIntersect = false;
    for (size_t i = 0; i < world->GetGridCount(); i++) {
        Intersection gridIntersection = { };
        if (SegmentIntersectsGrid(start, end, &world->GetGrids()[i], gridIntersection)) {
            didIntersect = true;
            if (gridIntersection.Time < nearestTime) {
                nearestTime = gridIntersection.Time;
                intersection = gridIntersection;
            }
        }
    }
    return didIntersect;
}

bool SweptBoundingBoxIntersectsGrid(const BoundingBox& bounds, const Vector2& end, const Grid* grid, Intersection& intersection) {

    // If the bounding box doesn't intersect the grid bounds, then we can skip checking the grid tiles
    if (!SweptBoundingBoxIntersectsBoundingBox(bounds, end, BoundingBox(Vector2(grid->GetPosition() + Vector2((float)grid->GetWidth(), (float)grid->GetHeight()) * 0.5f), Vector2((float)grid->GetWidth(), (float)grid->GetHeight()) * 0.5f), intersection)) {
        return false;
    }

    BoundingBox boundsRelative = BoundingBox(bounds.Position - grid->GetPosition(), bounds.HalfSize);
    Vector2 endRelative = end - grid->GetPosition();

    Vector2 delta = endRelative - boundsRelative.Position;
    int xdir = (delta.X >= 0) ? 1 : -1;
    int ydir = (delta.Y >= 0) ? 1 : -1;

    // Test the starting location
    // NOTE: This can be skipped if you assume that the box's starting position has already been checked
    // For example, when moving a character, we might assume that the last known position was not colliding
    for (int cellY = (int)floorf(boundsRelative.Position.Y - boundsRelative.HalfSize.Y); cellY <= (int)floorf(boundsRelative.Position.Y + boundsRelative.HalfSize.Y); cellY++) {
        if (cellY >= 0 && cellY < grid->GetHeight()) {
            for (int cellX = (int)floorf(boundsRelative.Position.X - boundsRelative.HalfSize.X); cellX <= (int)floorf(boundsRelative.Position.X + boundsRelative.HalfSize.X); cellX++) {
                if (cellX >= 0 && cellX < grid->GetWidth() && grid->GetTile(cellX, cellY).Collides) {
                    // We collide
                    intersection.EndPosition = bounds.Position;
                    intersection.Overlap = Vector2();
                    intersection.Time = 0.0f;
                    intersection.Normal = Vector2();
                    if (HMM_ABS(delta.X) > HMM_ABS(delta.Y)) {
                        intersection.Normal = Vector2((float)-xdir, 0.0f);
                    }
                    else {
                        intersection.Normal = Vector2(0.0f, (float)-ydir);
                    }

                    Vector2 tileCenter = Vector2((float)cellX + 0.5f, (float)cellY + 0.5f);
                    Vector2 toBounds = boundsRelative.Position - tileCenter;
                    if (toBounds.X > boundsRelative.HalfSize.X)
                        toBounds.X = boundsRelative.HalfSize.X;
                    else if (toBounds.X < -boundsRelative.HalfSize.X)
                        toBounds.X = -boundsRelative.HalfSize.X;

                    if (toBounds.Y > boundsRelative.HalfSize.Y)
                        toBounds.Y = boundsRelative.HalfSize.Y;
                    else if (toBounds.Y < -boundsRelative.HalfSize.Y)
                        toBounds.Y = -boundsRelative.HalfSize.Y;

                    intersection.ContactPoint = tileCenter + toBounds + grid->GetPosition();

                    return true;
                }
            }
        }
    }

    float cornerX = boundsRelative.Position.X + boundsRelative.HalfSize.X * xdir;
    float cornerY = boundsRelative.Position.Y + boundsRelative.HalfSize.Y * ydir;
    int cornerCellX = (int)floorf(cornerX);
    int cornerCellY = (int)floorf(cornerY);
    int endCellX = (int)floorf(endRelative.X + boundsRelative.HalfSize.X * xdir);
    int endCellY = (int)floorf(endRelative.Y + boundsRelative.HalfSize.Y * ydir);

    float dy; // The slope, or, how far in the y we move for each 1 unit of x
    if (endRelative.X != boundsRelative.Position.X) {
        dy = (endRelative.Y - boundsRelative.Position.Y) / (endRelative.X - boundsRelative.Position.X);
    }
    else {
        dy = INFINITY;
    }

    float dx; // How far in the x we move for each 1 unit of y
    if (endRelative.Y != boundsRelative.Position.Y) {
        dx = (endRelative.X - boundsRelative.Position.X) / (endRelative.Y - boundsRelative.Position.Y);
    }
    else {
        dx = INFINITY;
    }

    // Determine how far it will be until we hit the next cell in the X direction
    int nextX = xdir >= 0 ? (int)floorf(cornerX + 1.0f) : (int)floorf(cornerX);
    float stepX = nextX - cornerX; // How far in X to the next X coordinate
    float toNextX = sqrtf(stepX * stepX + (stepX * dy) * (stepX * dy)); // How far along the line to the next X coordinate
    float xIncrement = sqrtf(1.0f * 1.0f + dy * dy);

    // Determine how far it will be until we hit the next cell in the Y direction
    int nextY = ydir >= 0 ? (int)floorf(cornerY + 1.0f) : (int)floorf(cornerY);
    float stepY = nextY - cornerY; // How far in Y to the next Y coordinate
    float toNextY = sqrtf(stepY * stepY + (stepY * dx) * (stepY * dx)); // How far along the line to the next Y coordinate
    float yIncrement = sqrtf(dx * dx + 1.0f * 1.0f);

    // Keep stepping until we reach the destination cell
    while (cornerCellX != endCellX || cornerCellY != endCellY) {
        // Determine whether the next X value is closer, or the next Y value
        if (toNextX < toNextY) {
            // Move to the next X value
            cornerCellX += xdir;
            cornerX += stepX;
            cornerY += stepX * dy;

            // Adjust our movement plan along the other axis
            stepY -= stepX * dy; // Convert how far we moved in X to how far we moved in Y
            toNextY -= toNextX;

            // Calculate for the new next X value
            nextX = cornerCellX + xdir;
            stepX = 1.0f * xdir;
            toNextX = xIncrement;

            // Check the newly-occupied column of cells for collision
            int cellCount = cornerCellY - (int)floorf(cornerY - boundsRelative.HalfSize.Y * 2.0f * ydir);
            cellCount = 1 + HMM_ABS(cellCount);
            for (int i = 0; i < cellCount; i++) {
                int y = cornerCellY - i * ydir;
                if (cornerCellX >= 0 && cornerCellX < grid->GetWidth()
                    && y >= 0 && y < grid->GetHeight()
                    && grid->GetTile(cornerCellX, y).Collides) {

                    // We collide
                    intersection.EndPosition.X = cornerX - boundsRelative.HalfSize.X * xdir;
                    intersection.EndPosition.Y = cornerY - boundsRelative.HalfSize.Y * ydir;
                    intersection.Overlap = Vector2();
                    intersection.Normal = Vector2((float)-xdir, 0.0f);

                    Vector2 tileCenter = Vector2((float)cornerCellX + 0.5f, (float)y + 0.5f);
                    Vector2 toBounds = boundsRelative.Position - tileCenter;
                    if (toBounds.X > boundsRelative.HalfSize.X)
                        toBounds.X = boundsRelative.HalfSize.X;
                    else if (toBounds.X < -boundsRelative.HalfSize.X)
                        toBounds.X = -boundsRelative.HalfSize.X;

                    if (toBounds.Y > boundsRelative.HalfSize.Y)
                        toBounds.Y = boundsRelative.HalfSize.Y;
                    else if (toBounds.Y < -boundsRelative.HalfSize.Y)
                        toBounds.Y = -boundsRelative.HalfSize.Y;

                    intersection.ContactPoint = tileCenter + toBounds;
                    float total = sqrtf((endRelative.X - boundsRelative.Position.X) * (endRelative.X - boundsRelative.Position.X) + (endRelative.Y - boundsRelative.Position.Y) * (endRelative.Y - boundsRelative.Position.Y));
                    float distance = sqrtf((intersection.EndPosition.X - boundsRelative.Position.X) * (intersection.EndPosition.X - boundsRelative.Position.X) + (intersection.EndPosition.Y - boundsRelative.Position.Y) * (intersection.EndPosition.Y - boundsRelative.Position.Y));
                    intersection.Time = distance / total;

                    intersection.EndPosition += grid->GetPosition();
                    intersection.ContactPoint += grid->GetPosition();

                    return true;
                }
            }
        }
        else {
            // Move to the next Y value
            cornerCellY += ydir;
            cornerY += stepY;
            cornerX += stepY * dx;

            // Adjust our movement plan along the other axis
            stepX -= stepY * dx;
            toNextX -= toNextY;

            // Calculate for the next next Y value
            nextY = cornerCellY + ydir;
            stepY = 1.0f * ydir;
            toNextY = yIncrement;

            // Check the newly-occupied row of cells for collision
            int cellCount = cornerCellX - (int)floorf(cornerX - boundsRelative.HalfSize.X * 2.0f * xdir);
            cellCount = 1 + HMM_ABS(cellCount);
            for (int i = 0; i < cellCount; i++) {
                int x = cornerCellX - i * xdir;
                if (cornerCellY >= 0 && cornerCellY < grid->GetHeight()
                    && x >= 0 && x < grid->GetWidth()
                    && grid->GetTile(x, cornerCellY).Collides) {

                    // We collide
                    intersection.EndPosition.X = cornerX - boundsRelative.HalfSize.X * xdir;
                    intersection.EndPosition.Y = cornerY - boundsRelative.HalfSize.Y * ydir;
                    intersection.Overlap = Vector2();
                    intersection.Normal = Vector2(0.0f, (float)-ydir);

                    Vector2 tileCenter = Vector2((float)x + 0.5f, (float)cornerCellY + 0.5f);
                    Vector2 toBounds = boundsRelative.Position - tileCenter;
                    if (toBounds.X > boundsRelative.HalfSize.X)
                        toBounds.X = boundsRelative.HalfSize.X;
                    else if (toBounds.X < -boundsRelative.HalfSize.X)
                        toBounds.X = -boundsRelative.HalfSize.X;

                    if (toBounds.Y > boundsRelative.HalfSize.Y)
                        toBounds.Y = boundsRelative.HalfSize.Y;
                    else if (toBounds.Y < -boundsRelative.HalfSize.Y)
                        toBounds.Y = -boundsRelative.HalfSize.Y;

                    intersection.ContactPoint = tileCenter + toBounds;
                    float total = sqrtf((endRelative.X - boundsRelative.Position.X) * (endRelative.X - boundsRelative.Position.X) + (endRelative.Y - boundsRelative.Position.Y) * (endRelative.Y - boundsRelative.Position.Y));
                    float distance = sqrtf((intersection.EndPosition.X - boundsRelative.Position.X) * (intersection.EndPosition.X - boundsRelative.Position.X) + (intersection.EndPosition.Y - boundsRelative.Position.Y) * (intersection.EndPosition.Y - boundsRelative.Position.Y));
                    intersection.Time = distance / total;

                    intersection.EndPosition += grid->GetPosition();
                    intersection.ContactPoint += grid->GetPosition();

                    return true;
                }
            }
        }
    }

    // If we reach here, no cells were hit
    return false;
}

bool SweptBoundingBoxIntersectsWorld(const BoundingBox& bounds, const Vector2& end, const World* world, Intersection& intersection) {
    float nearestTime = 100.0f; // Anything larger than 1.0 works
    bool didIntersect = false;
    for (size_t i = 0; i < world->GetGridCount(); i++) {
        Intersection gridIntersection = { };
        if (SweptBoundingBoxIntersectsGrid(bounds, end, &world->GetGrids()[i], gridIntersection)) {
            didIntersect = true;
            if (gridIntersection.Time < nearestTime) {
                nearestTime = gridIntersection.Time;
                intersection = gridIntersection;
            }
        }
    }
    return didIntersect;
}