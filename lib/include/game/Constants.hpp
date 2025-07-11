#pragma once

#include <SFML/System/Vector2.hpp>

const sf::Vector2f INTERNAL_RESOLUTION = { 640.f, 360.f };
const sf::Color COLOR_BLACK = { 0, 0, 0, 255 };
const sf::Color COLOR_WHITE = { 255, 241, 232, 255 };
const sf::Color COLOR_DARK_GREY = { 95, 87, 79, 255 };
const sf::Color COLOR_DARK_BLUE = { 29, 43, 83, 255 };
const sf::Color COLOR_YELLOW = { 255, 236, 39, 255 };
const sf::Color COLOR_ORANGE = { 255, 163, 0, 255 };
const sf::Color COLOR_LIGHT_GREY = { 194, 195, 199, 255 };
const sf::Color COLOR_LIGHT_BLUE = { 41, 173, 255, 255 };
const sf::Color COLOR_LAVENDER = { 131, 118, 156, 255 };
const sf::Color COLOR_PINK = { 255, 119, 168, 255 };
const sf::Color COLOR_LIGHT_PEACH = { 255, 204, 170, 255 };
const sf::Color COLOR_DARK_PURPLE = { 126, 37, 83, 255 };
const sf::Color COLOR_RED = { 255, 0, 77, 255 };

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