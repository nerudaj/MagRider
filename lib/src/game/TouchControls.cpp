#include "game/TouchControls.hpp"
#include "game/Constants.hpp"
#include "gui/Icon.hpp"
#include "gui/Sizers.hpp"

static dgm::Circle
createMagnetButton(const sf::Vector2u& windowSize, bool left, float scaleFactor)
{
    const auto radius = windowSize.x / 15.f * scaleFactor;
    auto&& circle = dgm::Circle(
        { left ? radius : windowSize.x - radius, windowSize.y - radius },
        radius);
    return circle;
}

TouchControls::TouchControls(
    const dgm::ResourceManager& resmgr,
    Input& input,
    const InputSettings& settings,
    const sf::Vector2u& windowSize)
    : input(input)
    , pauseButtonSprite(resmgr.get<sf::Texture>("pixel-ui-icons.png"))
    , pauseButton({ 0.f, 0.f }, 0.f)
    , redButton(createMagnetButton(
          windowSize, "left"_true, settings.touchControlsSize))
    , blueButton(createMagnetButton(
          windowSize, "left"_false, settings.touchControlsSize))
{
    pauseButton.setRadius(Sizers::getBaseContainerHeight());
    pauseButton.setPosition(
        { pauseButton.getRadius(), pauseButton.getRadius() });

    const auto& frame = resmgr.get<dgm::Clip>("pixel-ui-icons.png.clip")
                            .getFrame(Icon::PauseFill);
    pauseButtonSprite.setTextureRect(frame);
    pauseButtonSprite.setScale({ pauseButton.getRadius() / frame.size.x,
                                 pauseButton.getRadius() / frame.size.y });
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

void TouchControls::regenerateButtons(
    const sf::Vector2u& windowSize, const InputSettings& settings)
{
    redButton =
        createMagnetButton(windowSize, "left"_true, settings.touchControlsSize);
    blueButton = createMagnetButton(
        windowSize, "left"_false, settings.touchControlsSize);
}
