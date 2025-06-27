#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Time.hpp>

class [[nodiscard]] TransitionAnimation final
{
public:
    TransitionAnimation(
        const float maxRadius,
        const sf::Time& duration,
        const sf::Color& color,
        const sf::Vector2f& position)
        : MAX_RADIUS(maxRadius), duration(duration)
    {
        circle.setPosition(position);
        circle.setFillColor(color);
    }

public:
    [[nodiscard]] bool isFinished() const noexcept
    {
        return elapsed >= duration;
    }

    void update(const sf::Time& dt);

    [[nodiscard]] sf::CircleShape& getDrawable() noexcept
    {
        return circle;
    }

    void reset()
    {
        elapsed = sf::Time::Zero;
    }

private:
    const float MAX_RADIUS;
    sf::Time duration;
    sf::Time elapsed;
    sf::CircleShape circle;
};