#pragma once

#include "game/Tile.hpp"
#include <SFML/System/Vector2.hpp>
#include <vector>

struct TileLayer
{
    unsigned id = 0;
    std::vector<Tile> tiles;
};

struct ObjectLayer
{
    std::vector<sf::Vector2f> positions;
};

struct TiledLevel
{
    unsigned width = 0;
    unsigned height = 0;
    unsigned tileWidth = 0;
    unsigned tileHeight = 0;
    std::vector<TileLayer> tileLayers;
    std::vector<ObjectLayer> objectLayers;
};
