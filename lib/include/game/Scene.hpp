#pragma once

#include <DGM/dgm.hpp>
#include <box2d/box2d.h>

struct [[nodiscard]] Scene final
{
    sf::Vector2f position;
    dgm::TileMap tileMap;
    dgm::Camera camera;
    dgm::Camera hudCamera;
    std::unique_ptr<b2World> world;
    b2Body& joe;
    int magnetPolarity = 0; // 0 off, 1 red, 2 blue
    std::vector<sf::Vector2f> redMagnetPositions;
    std::vector<sf::Vector2f> blueMagnetPositions;
};
