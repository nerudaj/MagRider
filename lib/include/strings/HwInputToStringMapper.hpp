#pragma once

#include "misc/Compatibility.hpp"
#include "types/Binding.hpp"
#include "types/SemanticTypes.hpp"
#include "types/StringTypes.hpp"
#include <DGM/dgm.hpp>
#include <string>

class HwInputToStringMapper
{
public:
    NODISCARD_RESULT StringType operator()(sf::Keyboard::Key key)
    {
        if (sf::Keyboard::Key::A <= key && key <= sf::Keyboard::Key::Z)
            return std::string(
                1, static_cast<char>('a' + std::to_underlying(key)));
        else if (key == sf::Keyboard::Key::Space)
            return "space";
        else if (key == sf::Keyboard::Key::Enter)
            return "enter";
        else if (key == sf::Keyboard::Key::Escape)
            return "escape";
        else if (key == sf::Keyboard::Key::Tab)
            return "tab";
        else if (key == sf::Keyboard::Key::Backspace)
            return "backspace";
        return uni::format("key {}", std::to_underlying(key));
    }

    NODISCARD_RESULT StringType operator()(sf::Mouse::Button btn)
    {
        if (btn == sf::Mouse::Button::Left)
            return "lmb";
        else if (btn == sf::Mouse::Button::Right)
            return "rmb";
        return uni::format("mouse {}", std::to_underlying(btn));
    }

    NODISCARD_RESULT StringType operator()(GamepadButton gamepadButtonIndex)
    {
        return uni::format("joy {}", gamepadButtonIndex.get());
    }

    NODISCARD_RESULT StringType
    operator()(std::pair<sf::Joystick::Axis, dgm::AxisHalf> axis)
    {
        auto axisName = [](sf::Joystick::Axis a) -> std::string
        {
            switch (a)
            {
            case sf::Joystick::Axis::X:
                return "x";
            case sf::Joystick::Axis::Y:
                return "y";
            case sf::Joystick::Axis::Z:
                return "z";
            case sf::Joystick::Axis::R:
                return "r";
            case sf::Joystick::Axis::U:
                return "u";
            case sf::Joystick::Axis::V:
                return "v";
            case sf::Joystick::Axis::PovX:
                return "povx";
            case sf::Joystick::Axis::PovY:
                return "povx";
            };

            return uni::format("Axis {}", std::to_underlying(a));
        };

        return uni::format(
            "{}{}",
            axisName(axis.first),
            axis.second == dgm::AxisHalf::Negative ? "-" : "+");
    }

    NODISCARD_RESULT StringType operator()(auto)
    {
        return "<not bound>";
    }

    NODISCARD_RESULT StringType getMapping(KmbBinding binding)
    {
        return std::visit(*this, binding);
    }

    NODISCARD_RESULT StringType getMapping(GamepadBinding binding)
    {
        return std::visit(*this, binding);
    }
};