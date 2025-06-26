#pragma once

#include <DGM/dgm.hpp>
#include <box2d/box2d.h>

constexpr const uintptr_t SPIKE = 1;
constexpr const uintptr_t FINISH = 2;

class SpikeContactListener : public b2ContactListener
{
public:
    void BeginContact(b2Contact* contact) override
    {
        auto a = contact->GetFixtureA();
        auto b = contact->GetFixtureB();

        uintptr_t userData = 0;
        if (a->IsSensor())
            userData = a->GetUserData().pointer;
        else if (b->IsSensor())
            userData = b->GetUserData().pointer;

        if (userData == SPIKE) died = true;
        if (userData == FINISH) won = true;
    }

    bool died = false;
    bool won = false;
};

struct [[nodiscard]] Magnet final
{
    sf::Vector2f position;
    int polarity = 0;
};

struct [[nodiscard]] Scene final
{
    std::unique_ptr<b2World> world;
    b2Body& joe;
    std::vector<Magnet> magnets;
    std::unique_ptr<SpikeContactListener> contactListener;
    int magnetPolarity = 0; // 0 off, 1 red, 2 blue
    bool playing = false;
    float timer = 0.f;
};
