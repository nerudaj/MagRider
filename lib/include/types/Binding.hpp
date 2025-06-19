#pragma once

#include "types/SemanticTypes.hpp"
#include <DGM/dgm.hpp>
#include <tuple>
#include <variant>

using KmbBinding =
    std::variant<sf::Keyboard::Key, sf::Mouse::Button, std::monostate>;

using GamepadBinding = std::variant<
    GamepadButton,
    std::pair<sf::Joystick::Axis, dgm::AxisHalf>,
    std::monostate>;

using Binding = std::tuple<KmbBinding, GamepadBinding>;

static_assert(std::is_default_constructible_v<KmbBinding>);
static_assert(std::is_default_constructible_v<GamepadBinding>);
static_assert(std::is_default_constructible_v<Binding>);
