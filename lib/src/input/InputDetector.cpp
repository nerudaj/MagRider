#include "input/InputDetector.hpp"

constexpr const float AXIS_THRESHOLD_NEG = -80.f;
constexpr const float AXIS_THRESHOLD_POS = 80.f;

void InputDetector::update(const dgm::Time& time)
{
    initialDelay -= time.getDeltaTime();
    if (initialDelay > 0.f) return;

    if (runMode == RunMode::Idle) return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
    {
        runMode = RunMode::Idle;
        cancel();
    }
    else if (runMode == RunMode::DetectingKmb)
    {
        if (tryKeyboard() == DetectionStatus::InputDetected
            || tryMouse() == DetectionStatus::InputDetected)
        {
            runMode = RunMode::Idle;
        }
    }
    else if (runMode == RunMode::DetectingGamepad)
    {
        if (tryGamepadAxis() == DetectionStatus::InputDetected
            || tryGamepadButton() == DetectionStatus::InputDetected)
        {
            runMode = RunMode::Idle;
        }
    }
}

InputDetector::DetectionStatus InputDetector::tryGamepadAxis()
{
    for (auto&& axisIdx = 0; axisIdx < sf::Joystick::AxisCount; ++axisIdx)
    {
        const auto axis = static_cast<sf::Joystick::Axis>(axisIdx);
        const auto position = sf::Joystick::getAxisPosition(0, axis);

        if (position < AXIS_THRESHOLD_NEG)
        {
            reportGamepad(std::pair { axis, dgm::AxisHalf::Negative });
            return DetectionStatus::InputDetected;
        }
        else if (position > AXIS_THRESHOLD_POS)
        {
            reportGamepad(std::pair { axis, dgm::AxisHalf::Positive });
            return DetectionStatus::InputDetected;
        }
    }

    return DetectionStatus::None;
}

InputDetector::DetectionStatus InputDetector::tryGamepadButton()
{
    for (auto&& btnIdx = 0u; btnIdx < sf::Joystick::ButtonCount; ++btnIdx)
    {
        const bool pressed = sf::Joystick::isButtonPressed(0, btnIdx);
        if (pressed)
        {
            reportGamepad(GamepadButton { btnIdx });
            return DetectionStatus::InputDetected;
        }
    }

    return DetectionStatus::None;
}

InputDetector::DetectionStatus InputDetector::tryKeyboard()
{
    for (auto&& keyIdx = 0; keyIdx < sf::Keyboard::KeyCount; ++keyIdx)
    {
        const auto key = static_cast<sf::Keyboard::Key>(keyIdx);
        const bool pressed = sf::Keyboard::isKeyPressed(key);

        if (pressed)
        {
            reportKmb(key);
            return DetectionStatus::InputDetected;
        }
    }

    return DetectionStatus::None;
}

InputDetector::DetectionStatus InputDetector::tryMouse()
{
    for (auto&& mbtnIdx = 0; mbtnIdx < sf::Mouse::ButtonCount; ++mbtnIdx)
    {
        const auto mbtn = static_cast<sf::Mouse::Button>(mbtnIdx);
        const bool pressed = sf::Mouse::isButtonPressed(mbtn);

        if (pressed)
        {
            reportKmb(mbtn);
            return DetectionStatus::InputDetected;
        }
    }

    return DetectionStatus::None;
}
