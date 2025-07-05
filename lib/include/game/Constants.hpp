#pragma once

#include <SFML/System/Vector2.hpp>

const sf::Vector2f INTERNAL_RESOLUTION = { 640.f, 360.f };
const sf::Color COLOR_BLACK = { 0, 0, 0, 255 };
const sf::Color COLOR_WHITE = { 255, 241, 232, 255 };
const sf::Color COLOR_DARK_GREY = { 95, 87, 79, 255 };
const sf::Color COLOR_YELLOW = { 255, 236, 39, 255 };
const sf::Color COLOR_ORANGE = { 255, 163, 0, 255 };

constexpr const float JOE_DENSITY = 0.5f;
constexpr const float JOE_FRICTION = 0.5f;
constexpr const float JOE_RESTITUTION = 0.4f;

constexpr const float MAGNET_RANGE = 6.f;
constexpr const float MAGNET_FORCE = 2.9f;

constexpr const int MAGNET_POLARITY_NONE = 0;
constexpr const int MAGNET_POLARITY_RED = 1;
constexpr const int MAGNET_POLARITY_BLUE = 2;

constexpr const int VELOCITY_ITERATIONS = 6;
constexpr const int POSITION_ITERATIONS = 2;