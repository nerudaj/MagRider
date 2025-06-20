#include "game/engine/GameRulesEngine.hpp"
#include <algorithm>
#include <limits>

constexpr float MAX_MAGNET_DISTANCE = 10.f;
constexpr float LINEAR_FORCE = 20.f;

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
    const auto vel = input.getHorizontalVelocity();
    scene.magnetPolarity = vel == 0.f ? 0 : vel < 0.f ? 1 : 2;

    sf::Vector2f totalForce = {};
    if (scene.magnetPolarity != 0)
    {
        for (auto&& magnetPos : scene.redMagnetPositions)
        {
            const auto direction = scene.joe.GetPosition() - magnetPos;
            if (direction.length() < MAX_MAGNET_DISTANCE)
            {
                totalForce += direction.normalized() * LINEAR_FORCE
                              * (scene.magnetPolarity == 1 ? 1.f : -1.f);
            }
        }

        for (auto&& magnetPos : scene.blueMagnetPositions)
        {
            const auto direction = scene.joe.GetPosition() - magnetPos;
            if (direction.length() < MAX_MAGNET_DISTANCE)
            {
                totalForce += direction.normalized() * LINEAR_FORCE
                              * (scene.magnetPolarity == 1 ? -1.f : 1.f);
            }
        }
    }

    scene.joe.ApplyForceToCenter(b2Vec2(totalForce.x, totalForce.y), true);

    scene.world->Step(time.getDeltaTime(), 6, 2);
}
