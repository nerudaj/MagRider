#pragma once

#include <box2d/box2d.h>
#include <SFML/System/Vector2.hpp>

/**
 * @brief A utility class for converting between Box2D and SFML coordinate systems.
 * 
 * Box2D uses meters as its unit of measurement, while SFML uses pixels. This class
 * defines a ratio between the two and provides methods to convert coordinates.
 */
class CoordConverter {
public:
    static sf::Vector2f worldToScreen(const b2Vec2& box2dVec) {
        return sf::Vector2f(box2dVec.x * PIXELS_PER_METER, box2dVec.y * PIXELS_PER_METER);
    }

    static b2Vec2 screenToWorld(const sf::Vector2f& sfmlVec) {
        return b2Vec2(sfmlVec.x / PIXELS_PER_METER, sfmlVec.y / PIXELS_PER_METER);
    }

    static float worldToScreen(float value) {
        return value * PIXELS_PER_METER;
    }

    static float screenToWorld(float value) {
        return value / PIXELS_PER_METER;
    }

public:
    static constexpr float PIXELS_PER_METER = 32.0f;
};
