#include "game/engine/RenderingEngine.hpp"
#include "game/Constants.hpp"
#include "gui/Sizers.hpp"
#include "misc/Compatibility.hpp"
#include "misc/CoordConverter.hpp"
#include "misc/Utility.hpp"
#include "types/SemanticTypes.hpp"

static dgm::Camera createFullscreenCamera(
    const sf::Vector2f& currentResolution,
    const sf::Vector2f& desiredResolution)
{
    auto&& viewport = sf::FloatRect {
        { 0.f, 0.f },
        { 1.f, 1.f },
    };

    const auto&& desiredAspectRatio = desiredResolution.x / desiredResolution.y;
    const auto&& currentAspectRatio = currentResolution.x / currentResolution.y;

    if (currentAspectRatio > desiredAspectRatio)
    {
        // Narrow desired into wider current
        viewport.size.x =
            desiredAspectRatio * currentResolution.y / currentResolution.x;
        viewport.position.x = (1.f - viewport.size.x) / 2.f;
    }
    else if (currentAspectRatio < desiredAspectRatio)
    {
        // Wider desired into narrower current
        viewport.size.y =
            currentResolution.x / desiredAspectRatio / currentResolution.y;
        viewport.position.y = (1.f - viewport.size.y) / 2.f;
    }

    return dgm::Camera(viewport, sf::Vector2f(desiredResolution));
}

static auto setAndGetSpritesheet(
    dgm::TextureAtlas& atlas,
    const sf::Texture& texture,
    const dgm::AnimationStates& states)
{
    auto locator = atlas.addSpritesheet(texture, states);
    return atlas.getAnimationStates(locator.value());
}

static auto setAndGetTileset(
    dgm::TextureAtlas& atlas, const sf::Texture& texture, const dgm::Clip& clip)
{
    auto locator = atlas.addTileset(texture, clip);
    return atlas.getClip(locator.value());
}

RenderingEngine::RenderingEngine(
    dgm::Window& window,
    dgm::ResourceManager& resmgr,
    const VideoSettings& settings,
    const StringProvider& strings,
    Scene& scene,
    const TiledLevel& level,
    const GameConfig& config) noexcept
    // Dependencies
    : window(window)
    , settings(settings)
    , strings(strings)
    , scene(scene)
    // Atlas properties
    , atlas(1024, 1024)
    , ballAnimationStates(setAndGetSpritesheet(
          atlas,
          resmgr.get<sf::Texture>("ball.png"),
          resmgr.get<dgm::AnimationStates>("ball.png.anim")))
    , magnetLineAnimationStates(setAndGetSpritesheet(
          atlas,
          resmgr.get<sf::Texture>("lines.png"),
          resmgr.get<dgm::AnimationStates>("lines.png.anim")))
    , tileset(setAndGetTileset(
          atlas,
          resmgr.get<sf::Texture>(config.tilesetName),
          resmgr.get<dgm::Clip>(config.tilesetName + ".clip")))
    // Non-drawables
    , boxDebugRenderer(window)
    , backgroundCamera(createFullscreenCamera(
          sf::Vector2f(window.getSize()), INTERNAL_RESOLUTION))
    , worldCamera(createFullscreenCamera(
          sf::Vector2f(window.getSize()), INTERNAL_RESOLUTION))
    , hudCamera(
          sf::FloatRect { { 0.f, 0.f }, { 1.f, 1.f } },
          sf::Vector2f(window.getSize()))
    , animation(2, 10)
    , joeSkinName(config.joeSkinName)

    // Drawables
    , text(resmgr.get<sf::Font>("pico-8.ttf"))
    , tileMap(dgm::TileMap(atlas.getTexture(), tileset))
    , sprite(atlas.getTexture())
    , line(atlas.getTexture())
    , background(resmgr.get<sf::Texture>(config.backgroundName))
    , joeAnimation(ballAnimationStates, 15)
{
    tileMap.build(
        { level.tileWidth, level.tileHeight },
        level.tileLayers.front().tiles
            | std::views::transform([](Tile tile)
                                    { return std::to_underlying(tile) - 1; })
            | uniranges::to<std::vector>(),
        { level.width, level.height });

    setJoeIdleState();

    sprite.setOrigin(sf::Vector2f {
        ballAnimationStates.begin()->second.getFrameSize() / 2u });
    spriteOutline.setRadius(sprite.getOrigin().x);
    spriteOutline.setOrigin(sprite.getOrigin());
    spriteOutline.setOutlineThickness(3.f);
    spriteOutline.setFillColor(sf::Color::Transparent);
    scene.world->SetDebugDraw(&boxDebugRenderer);
    boxDebugRenderer.SetFlags(b2Draw::e_shapeBit);

    line.setOrigin({ 0.f, 12.f });

    text.setOutlineColor(COLOR_BLACK);
    text.setOutlineThickness(0.5f);
    text.setFillColor(COLOR_WHITE);
    text.setLineSpacing(1.1f);

    resmgr.getMutable<sf::Font>("pico-8.ttf").setSmooth(false);
}

void RenderingEngine::update(const dgm::Time& time)
{
    animation.update(time);

    if (joeAnimation.update(time) == dgm::Animation::PlaybackStatus::Finished)
    {
        setJoeIdleState();
    }

    fpsCounter.update(time.getDeltaTime());

    timeToBlink -= time.getElapsed();
    if (timeToBlink < sf::Time::Zero)
    {
        joeAnimation.setState(joeSkinName + "_joe_blink", "looping"_false);
    }
}

static sf::Vector2f operator-(const sf::Vector2f& a, const b2Vec2& b)
{
    return {
        a.x - b.x,
        a.y - b.y,
    };
}

void RenderingEngine::draw(bool paused)
{
    window.setViewFromCamera(backgroundCamera);
    window.draw(background);

    window.setViewFromCamera(worldCamera);
    renderWorld();

    window.setViewFromCamera(hudCamera);

    if (!paused) renderHUD();
}

void RenderingEngine::renderWorld()
{
    auto joePos = CoordConverter::worldToScreen(scene.joe.GetPosition());

    worldCamera.setPosition(joePos);
    sprite.setTextureRect(joeAnimation.getCurrentFrame());
    sprite.setPosition(joePos);
    sprite.setRotation(sf::radians(scene.joe.GetAngle()));
    spriteOutline.setPosition(joePos);

    if (scene.magnetPolarity == MAGNET_POLARITY_NONE)
        spriteOutline.setOutlineColor(sf::Color::Transparent);
    else if (scene.magnetPolarity == MAGNET_POLARITY_RED)
        spriteOutline.setOutlineColor(sf::Color::Red);
    else if (scene.magnetPolarity == MAGNET_POLARITY_BLUE)
        spriteOutline.setOutlineColor(sf::Color::Blue);

    window.draw(tileMap);
    window.draw(spriteOutline);
    window.draw(sprite);

    if (scene.magnetPolarity != 0)
    {
        for (auto&& magnet : scene.magnets)
        {
            const auto direction = magnet.position - scene.joe.GetPosition();
            if (direction.length() < MAGNET_RANGE)
            {
                renderMagnetLine(joePos, direction);
            }
        }
    }

    if (settings.renderColliders) scene.world->DebugDraw();

    for (auto&& label : scene.texts)
    {
        text.setCharacterSize(10);
        text.setString(strings.getString(label.textId));
        text.setPosition(label.position);
        window.draw(text);
    }
}

void RenderingEngine::renderMagnetLine(
    const sf::Vector2f& joeScreenPos, const sf::Vector2f& direction)
{
    const float JOE_WORLD_RADIUS = 0.5f;
    const float JOE_SCREEN_RADIUS = 16.f;
    const float MAGLINE_SCREEN_LENGTH = 48.f;

    line.setPosition(
        joeScreenPos + dgm::Math::toUnit(direction) * JOE_SCREEN_RADIUS);
    line.setRotation(dgm::Math::cartesianToPolar(direction).angle);
    line.setScale(
        { CoordConverter::worldToScreen(direction.length() - JOE_WORLD_RADIUS)
              / MAGLINE_SCREEN_LENGTH,
          1.f });
    line.setTextureRect(
        magnetLineAnimationStates
            [scene.magnetPolarity == MAGNET_POLARITY_RED ? "red" : "blue"]
                .getFrame(animation.getFrame()));
    window.draw(line);
}

void RenderingEngine::renderHUD()
{
    const auto baseFontSize = Sizers::getBaseFontSize();
    text.setCharacterSize(baseFontSize);

    if (settings.showFps)
    {
        text.setString(fpsCounter.getText());
        text.setPosition({
            static_cast<float>(window.getSize().x)
                - text.getGlobalBounds().size.x - 10.f,
            20.f + baseFontSize,
        });
        window.draw(text);
    }

    text.setString(Utility::formatTime(scene.timer));
    text.setPosition({
        static_cast<float>(window.getSize().x) - text.getGlobalBounds().size.x
            - 10.f,
        10.f,
    });
    window.draw(text);

    if (!scene.playing)
    {
        text.setCharacterSize(baseFontSize * 2);

#ifdef ANDROID
        text.setString(strings.getString(StringId::TouchToStart));
#else
        text.setString(strings.getString(StringId::SpaceToStart));
#endif
        text.setPosition(
            sf::Vector2f(window.getSize()) / 2.f
            - text.getGlobalBounds().size / 2.f);
        window.draw(text);
    }
}

void RenderingEngine::setJoeIdleState()
{
    joeAnimation.setState(joeSkinName + "_joe_idle", "looping"_true);
    timeToBlink = sf::seconds(static_cast<float>(rand() % 5));
}
