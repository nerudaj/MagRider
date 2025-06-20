#pragma once

#include <DGM/dgm.hpp>
#include <box2d/box2d.h>

class SpikeContactListener : public b2ContactListener
{
public:
    void BeginContact(b2Contact* contact) override
    {
        auto a = contact->GetFixtureA();
        auto b = contact->GetFixtureB();

        died = a->IsSensor() || b->IsSensor();
    }

    bool died = false;
};

struct [[nodiscard]] Scene final
{
    sf::Vector2f position;
    dgm::TileMap tileMap;
    dgm::Camera camera;
    dgm::Camera hudCamera;
    std::unique_ptr<SpikeContactListener> contactListener;
    std::unique_ptr<b2World> world;
    b2Body& joe;
    int magnetPolarity = 0; // 0 off, 1 red, 2 blue
    std::vector<sf::Vector2f> redMagnetPositions;
    std::vector<sf::Vector2f> blueMagnetPositions;
    bool playing = false;
};
