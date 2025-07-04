#include "game/engine/RenderingEngine.hpp"
#include "game/Constants.hpp"
#include "misc/Compatibility.hpp"
#include "misc/CoordConverter.hpp"
#include "misc/Utility.hpp"

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
    const TiledLevel& level) noexcept
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
          resmgr.get<sf::Texture>("set.png"),
          resmgr.get<dgm::Clip>("set.png.clip")))
    // Non-drawables
    , boxDebugRenderer(window)
    , worldCamera(createFullscreenCamera(
          sf::Vector2f(window.getSize()), INTERNAL_RESOLUTION))
    , hudCamera(
          sf::FloatRect { { 0.f, 0.f }, { 1.f, 1.f } },
          sf::Vector2f(window.getSize()))
    , animation(2, 10)

    // Drawables
    , text(resmgr.get<sf::Font>("pico-8.ttf"))
    , tileMap(dgm::TileMap(atlas.getTexture(), tileset))
    , sprite(atlas.getTexture())
    , line(atlas.getTexture())
    , background(resmgr.get<sf::Texture>("background-forest.png"))
{
    tileMap.build(
        { level.tileWidth, level.tileHeight },
        level.tileLayers.front().tiles
            | std::views::transform([](Tile tile)
                                    { return std::to_underlying(tile) - 1; })
            | uniranges::to<std::vector>(),
        { level.width, level.height });

    sprite.setTextureRect(ballAnimationStates["base_joe_idle"].getFrame(0));
    sprite.setOrigin(sf::Vector2f {
        ballAnimationStates["base_joe_idle"].getFrameSize() / 2u });
    spriteOutline.setRadius(sprite.getOrigin().x);
    spriteOutline.setOrigin(sprite.getOrigin());
    spriteOutline.setOutlineThickness(3.f);
    scene.world->SetDebugDraw(&boxDebugRenderer);
    boxDebugRenderer.SetFlags(b2Draw::e_shapeBit);

    background.setOrigin(INTERNAL_RESOLUTION / 2.f);

    line.setOrigin({ 0.f, 12.f });
}

void RenderingEngine::update(const dgm::Time& time)
{
    animation.update(time);
    fpsCounter.update(time.getDeltaTime());
}

static sf::Vector2f operator-(const b2Vec2& a, sf::Vector2f& b)
{
    return {
        a.x - b.x,
        a.y - b.y,
    };
}

void RenderingEngine::draw(dgm::Window& _window)
{
    window.setViewFromCamera(worldCamera);
    RenderWorld();

    window.setViewFromCamera(hudCamera);
    RenderHUD();
}

void RenderingEngine::RenderWorld()
{
    auto joePos = CoordConverter::worldToScreen(scene.joe.GetPosition());
    auto roundedJoePos = sf::Vector2f(sf::Vector2i(joePos));

    worldCamera.setPosition(roundedJoePos);
    background.setPosition(joePos);
    sprite.setPosition(joePos);
    sprite.setRotation(sf::radians(scene.joe.GetAngle()));
    spriteOutline.setPosition(joePos);

    if (scene.magnetPolarity == 0)
        spriteOutline.setOutlineColor(sf::Color::Transparent);
    else if (scene.magnetPolarity == 1)
        spriteOutline.setOutlineColor(sf::Color::Red);
    else if (scene.magnetPolarity == 2)
        spriteOutline.setOutlineColor(sf::Color::Blue);

    window.draw(background);
    window.draw(tileMap);
    window.draw(spriteOutline);
    window.draw(sprite);

    if (scene.magnetPolarity != 0)
    {
        for (auto&& magnet : scene.magnets)
        {
            const auto direction = scene.joe.GetPosition() - magnet.position;
            if (direction.length() < 6.f)
            {
                const auto screenMagnetPos = magnet.position * 32.f;
                line.setPosition(joePos);
                line.setRotation(
                    dgm::Math::cartesianToPolar(screenMagnetPos - joePos)
                        .angle);
                line.setScale({ (direction.length() * 32.f) / 48.f, 1.f });
                line.setTextureRect(magnetLineAnimationStates
                                        [magnet.polarity == 2 ? "red" : "blue"]
                                            .getFrame(animation.getFrame()));

                /* auto vertices = std::array {
                                    sf::Vertex { .position = joePos },
                                    sf::Vertex {},
                                };
                                window.getSfmlWindowContext().draw(
                                    vertices.data(), 2u,
                   sf::PrimitiveType::Lines);*/
                window.draw(line);
            }
        }
    }

    if (settings.renderColliders) scene.world->DebugDraw();
}

void RenderingEngine::RenderHUD()
{
    text.setPosition({ 10.f, 10.f });
    text.setString(fpsCounter.getText());
    window.draw(text);

    if (!scene.playing)
    {
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

    text.setString(Utility::formatTime(scene.timer));
    text.setPosition({
        static_cast<float>(window.getSize().x) - text.getGlobalBounds().size.x
            - 10.f,
        10.f,
    });
    window.draw(text);
}
