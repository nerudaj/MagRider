#pragma once

#include <SFML/System/Vector2.hpp>

struct [[nodiscard]] VideoSettings final
{
    sf::Vector2u resolution = { 1280, 720 };
    bool fullscreen = false;
};
