#include "game/TouchControls.hpp"
#include "gui/Icon.hpp"
#include "gui/Sizers.hpp"

static dgm::Circle createMagnetButton(const sf::Vector2u& windowSize, bool left)
{
    const auto radius = windowSize.x / 15.f;
    auto&& circle = dgm::Circle(
        { left ? radius : windowSize.x - radius, windowSize.y - radius },
        radius);
    return circle;
}

TouchControls::TouchControls(
    const dgm::ResourceManager& resmgr,
    Input& input,
    const sf::Vector2u& windowSize)
    : input(input)
    , pauseButtonSprite(resmgr.get<sf::Texture>("pixel-ui-icons.png"))
    , pauseButton({ 0.f, 0.f }, 0.f)
    , redButton(createMagnetButton(windowSize, "left"_true))
    , blueButton(createMagnetButton(windowSize, "left"_false))
{
    pauseButton.setRadius(Sizers::getBaseContainerHeight() / 2.f);
    pauseButton.setPosition(
        { pauseButton.getRadius(), pauseButton.getRadius() });

    const auto& frame = resmgr.get<dgm::Clip>("pixel-ui-icons.png.clip")
                            .getFrame(Icon::PauseFill);
    pauseButtonSprite.setTextureRect(frame);
    pauseButtonSprite.setScale(
        { pauseButton.getRadius() * 2.f / frame.size.x,
          pauseButton.getRadius() * 2.f / frame.size.y });
    pauseButtonSprite.setOrigin(sf::Vector2f(frame.size / 2));
    pauseButtonSprite.setPosition(pauseButton.getPosition());
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

sf::Color makeSemiTransparentColor(const sf::Color& color)
{
    return sf::Color(color.r, color.g, color.b, 128);
}

void TouchControls::draw(dgm::Window& window)
{
#ifdef ANDROID
    window.draw(pauseButtonSprite);
    redButton.debugRender(
        window,
        makeSemiTransparentColor(
            input.isMagnetizingRed() ? COLOR_DARK_PURPLE : COLOR_RED));
    blueButton.debugRender(
        window,
        makeSemiTransparentColor(
            input.isMagnetizingBlue() ? COLOR_DARK_BLUE : COLOR_LIGHT_BLUE));
#endif
}
