#include "game/TransitionAnimation.hpp"

void TransitionAnimation::update(const sf::Time& dt)
{
    elapsed += dt;
    const float f = std::min(1.f, elapsed.asSeconds() / duration.asSeconds());
    circle.setRadius(std::lerp(0.f, MAX_RADIUS, f));
    circle.setOrigin({ circle.getRadius(), circle.getRadius() });
}
