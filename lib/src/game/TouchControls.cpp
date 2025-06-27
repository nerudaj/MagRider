#include "game/TouchControls.hpp"

static dgm::Circle createMagnetButton(const sf::Vector2u& windowSize, bool left)
{
    const auto radius = windowSize.x / 15.f;
    auto&& circle = dgm::Circle(
        { left ? radius : windowSize.x - radius, windowSize.y - radius },
        radius);
    return circle;
}

TouchControls::TouchControls(Input& input, const sf::Vector2u& windowSize)
    : input(input)
    , pauseButton(
          { windowSize.x / 20.f, windowSize.x / 20.f }, windowSize.x / 20.f)
    , redButton(createMagnetButton(windowSize, "left"_true))
    , blueButton(createMagnetButton(windowSize, "left"_false))
{
}

void TouchControls::processEvent(const sf::Event::TouchBegan& e)
{
    auto position = e.position;

    const auto action = [&]
    {
        if (dgm::Collision::basic(redButton, e.position))
            return InputKind::MagnetizeRed;
        else if (dgm::Collision::basic(blueButton, e.position))
            return InputKind::MagnetizeBlue;
        else if (dgm::Collision::basic(pauseButton, e.position))
            return InputKind::BackButton;
        return InputKind::Jump;
    }();

    fingerToAction[e.finger] = action;

    input.toggleInput(action, true);
}

void TouchControls::processEvent(const sf::Event::TouchEnded& e)
{
    input.toggleInput(fingerToAction[e.finger], false);
}

void TouchControls::draw(dgm::Window& window)
{
#ifdef ANDROID
    pauseButton.debugRender(app.window, sf::Color(96, 96, 96, 128));
    redButton.debugRender(app.window, sf::Color(255, 0, 0, 128));
    blueButton.debugRender(app.window, sf::Color(0, 0, 255, 128));
#endif
}
