#include "game/engine/GameRulesEngine.hpp"
#include <algorithm>
#include <limits>

constexpr float MAX_MAGNET_DISTANCE = 6.f;
constexpr float LINEAR_FORCE = 2.f;

sf::Vector2f aggregateMagnetForces(
    const sf::Vector2f& joePos,
    const std::vector<sf::Vector2f>& magnetPositions)
{
    sf::Vector2f totalForce = {};
    for (auto&& magnetPos : magnetPositions)
    {
        const auto direction = joePos - magnetPos;
        if (direction.length() > MAX_MAGNET_DISTANCE)
        {
            totalForce += direction.normalized() * LINEAR_FORCE;
        }
    }

    return totalForce;
}

static sf::Vector2f operator-(const b2Vec2& a, sf::Vector2f& b)
{
    return {
        a.x - b.x,
        a.y - b.y,
    };
}

void GameRulesEngine::update(const dgm::Time& time)
{
    if (!scene.playing)
    {
        scene.playing = input.shouldStart();
        return;
    }

    if (input.isMagnetizingRed())
        scene.magnetPolarity = 1;
    else if (input.isMagnetizingBlue())
        scene.magnetPolarity = 2;
    else
        scene.magnetPolarity = 0;

    sf::Vector2f totalForce = {};
    if (scene.magnetPolarity != 0)
    {
        for (auto&& magnet : scene.magnets)
        {
            const auto direction = scene.joe.GetPosition() - magnet.position;
            if (direction.length() < MAX_MAGNET_DISTANCE)
            {
                totalForce +=
                    direction.normalized() * LINEAR_FORCE
                    * (scene.magnetPolarity == magnet.polarity ? -1.f : 1.f);
            }
        }
    }

    scene.joe.ApplyForceToCenter(b2Vec2(totalForce.x, totalForce.y), true);

    scene.world->Step(time.getDeltaTime(), 6, 2);
}
