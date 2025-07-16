#include "input/VirtualCursor.hpp"

void VirtualCursor::update(const dgm::Time& time, const float cursorSpeed)
{
    const auto origPosition = position;

    const auto cursorDelta = input.getCursorDelta();
    const auto mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
    if (cursorDelta != sf::Vector2f {})
    {
        position = clampPositionByWindow(
            position + cursorDelta * cursorSpeed * time.getDeltaTime(),
            window.getSize());
    }
    else if (mousePos != sf::Vector2f {})
    {
        position = mousePos;
    }

    sf::Mouse::setPosition(sf::Vector2i(position), window);

    if (origPosition == position)
        timeSinceLastChange += time.getElapsed();
    else
        timeSinceLastChange = sf::seconds(0.f);
}

void VirtualCursor::draw()
{
    if (timeSinceLastChange > sf::seconds(5)) return;

    sprite.setPosition(position);
    window.draw(sprite);
}

sf::Vector2f VirtualCursor::clampPositionByWindow(
    const sf::Vector2f& currentPosition, const sf::Vector2u& windowSize)
{
    return sf::Vector2f {
        std::clamp(currentPosition.x, 0.f, static_cast<float>(windowSize.x)),
        std::clamp(currentPosition.y, 0.f, static_cast<float>(windowSize.y))
    };
}
