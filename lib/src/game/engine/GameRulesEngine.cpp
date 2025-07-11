#include "game/engine/GameRulesEngine.hpp"
#include "game/Constants.hpp"
#include <algorithm>
#include <limits>

static sf::Vector2f operator-(const b2Vec2& a, const sf::Vector2f& b)
{
    return {
        a.x - b.x,
        a.y - b.y,
    };
}

static sf::Vector2f aggregateMagnetForces(
    const b2Vec2& joePos,
    const int joePolarity,
    const std::vector<Magnet>& magnets)
{
    sf::Vector2f totalForce = {};
    if (joePolarity == MAGNET_POLARITY_NONE) return totalForce;

    for (auto&& magnet : magnets)
    {
        const auto direction = joePos - magnet.position;
        if (direction.length() < MAGNET_RANGE)
        {
            totalForce += direction.normalized() * MAGNET_FORCE
                          * (joePolarity == magnet.polarity ? 1.f : -1.f);
        }
    }

    return totalForce;
}

void GameRulesEngine::update(const dgm::Time& time)
{
    if (!scene.playing)
    {
        scene.playing = input.shouldStart();
        return;
    }

    if (!scene.contactListener->died && !scene.contactListener->won)
        scene.timer += time.getDeltaTime();

    if (input.isMagnetizingRed())
    {
        scene.magnetPolarity = MAGNET_POLARITY_RED;
    }
    else if (input.isMagnetizingBlue())
    {
        scene.magnetPolarity = MAGNET_POLARITY_BLUE;
    }
    else
        scene.magnetPolarity = MAGNET_POLARITY_NONE;

    sf::Vector2f totalForce = aggregateMagnetForces(
        scene.joe.GetPosition(), scene.magnetPolarity, scene.magnets);

    // Only trigger sounds when magnet is affecting joe
    if (totalForce.length() > 0.f)
    {
        if (scene.magnetPolarity == MAGNET_POLARITY_RED)
            audioEventQueue.pushEvent<JoeMagnetizedToRedAudioEvent>();
        else
            audioEventQueue.pushEvent<JoeMagnetizedToBlueAudioEvent>();
    }

    scene.joe.ApplyForceToCenter(b2Vec2(totalForce.x, totalForce.y), true);

    scene.world->Step(
        time.getDeltaTime(), VELOCITY_ITERATIONS, POSITION_ITERATIONS);
}
