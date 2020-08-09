#pragma once

#include "glad.h"

/**
 * @brief Contains information on the size and format of a vertex attribute.
 *
 * Vertex attributes are composed of 1-4 elements of the same type.
 */
struct GLVertexAttribute {
    /**
     * @brief The number of components in this vertex attribute.
     *
     * This value can range from 1 through 4.
     */
    int ComponentCount;

    /**
     * @brief The format of the components in this vertex attribute.
     *
     * This value must be one of the following:
     *  - `GL_BYTE`
     *  - `GL_UNSIGNED_BYTE`
     *  - `GL_SHORT`
     *  - `GL_UNSIGNED_SHORT`
     *  - `GL_INT`
     *  - `GL_UNSIGNED_INT`
     *  - `GL_HALF_FLOAT`
     *  - `GL_FLOAT`
     *  - `GL_DOUBLE`
     *  - `GL_FIXED`
     */
    unsigned int ComponentFormat;

    /**
     * @brief The size of the components in this vertex attribute.
     *
     * This must correspond to the value of `ComponentFormat`.
     */
    size_t ComponentSize;
};