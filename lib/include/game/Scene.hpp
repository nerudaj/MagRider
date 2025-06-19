#pragma once

#include <DGM/dgm.hpp>

struct [[nodiscard]] Scene final
{
    sf::Vector2f position;
    dgm::TileMap tileMap;
    dgm::Camera camera;
};
