#pragma once

#include <stddef.h>

class Allocator;
class GLTexture;

/**
 * @brief Specifies how to use a GLTexture as a tilemap.
 *
 */
class TilePalette {
private:
    Allocator& _Allocator;
    const GLTexture* _Texture; // The texture to draw with
    float _TileWidth; // In UV space
    float _TileHeight; // In UV space
    size_t _Stride; // Tiles per row

public:
    TilePalette(Allocator& allocator, const char* data, size_t dataLength);
    ~TilePalette() { }

    /**
     * @brief Gets the GLTexture used for drawing tiles with this TilePalette.
     *
     * @return The GLTexture used for drawing.
     */
    inline const GLTexture* GetTexture() const { return this->_Texture; }

    /**
     * @brief Gets the width of a single tile, in UV space.
     *
     * @return The width of a single tile.
     */
    inline float GetTileWidth() const { return this->_TileWidth; }

    /**
     * @brief Get the height of a single tile, in UV space.
     *
     * @return The height of a single tile.
     */
    inline float GetTileHeight() const { return this->_TileHeight; }

    inline size_t GetStride() const { return this->_Stride; }
};