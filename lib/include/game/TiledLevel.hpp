#pragma once

#include "game/Tile.hpp"
#include <SFML/System/Vector2.hpp>
#include <vector>

struct TileLayer
{
    unsigned id = 0;
    std::vector<Tile> tiles;
};

enum class ObjectKind
{
    Point,
    Text
};

struct ObjectData
{
    sf::Vector2f position;
    ObjectKind kind;
    std::string data;
};

struct ObjectLayer
{
    std::vector<ObjectData> objects;
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
