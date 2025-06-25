#include "game/engine/RenderingEngine.hpp"
#include "misc/Compatibility.hpp"
#include "misc/CoordConverter.hpp"

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
        viewport.size.x = desiredResolution.y / currentResolution.y;
        viewport.position.x = (1.f - viewport.size.x) / 2.f;
    }
    else if (currentAspectRatio < desiredAspectRatio)
    {
        // Wider desired into narrower current
        viewport.size.y = desiredResolution.x / currentResolution.x;
        viewport.position.y = (1.f - viewport.size.y) / 2.f;
    }

    return dgm::Camera(viewport, sf::Vector2f(desiredResolution));
}

RenderingEngine::RenderingEngine(
    dgm::Window& window,
    dgm::ResourceManager& resmgr,
    const VideoSettings& settings,
    Scene& scene,
    const TiledLevel& level) noexcept
    : window(window)
    , settings(settings)
    , scene(scene)
    , boxDebugRenderer(window)
    , worldCamera(createFullscreenCamera(
          sf::Vector2f(window.getSize()), { 640.f, 360.f }))
    , hudCamera(
          sf::FloatRect { { 0.f, 0.f }, { 1.f, 1.f } },
          sf::Vector2f(window.getSize()))
    , text(resmgr.get<sf::Font>("ChunkFive-Regular.ttf"))
    , tileMap(dgm::TileMap(
          resmgr.get<sf::Texture>("set.png"),
          resmgr.get<dgm::Clip>("set.png.clip")))
    , sprite(resmgr.get<sf::Texture>("joe.png"))
{
    tileMap.build(
        { level.tileWidth, level.tileHeight },
        level.tileLayers.front().tiles
            | std::views::transform([](Tile tile)
                                    { return std::to_underlying(tile) - 1; })
            | uniranges::to<std::vector>(),
        { level.width, level.height });

    sprite.setOrigin(sf::Vector2f { sprite.getTexture().getSize() / 2u });
    spriteOutline.setRadius(sprite.getOrigin().x);
    spriteOutline.setOrigin(sprite.getOrigin());
    spriteOutline.setOutlineThickness(3.f);
    scene.world->SetDebugDraw(&boxDebugRenderer);
    boxDebugRenderer.SetFlags(b2Draw::e_shapeBit);
}

void RenderingEngine::update(const dgm::Time& time)
{
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
    auto joePos = CoordConverter::worldToScreen(scene.joe.GetPosition());

    worldCamera.setPosition(joePos);
    sprite.setPosition(joePos);
    sprite.setRotation(sf::radians(scene.joe.GetAngle()));
    spriteOutline.setPosition(joePos);

    if (scene.magnetPolarity == 0)
        spriteOutline.setOutlineColor(sf::Color::Transparent);
    else if (scene.magnetPolarity == 1)
        spriteOutline.setOutlineColor(sf::Color::Red);
    else if (scene.magnetPolarity == 2)
        spriteOutline.setOutlineColor(sf::Color::Blue);

    window.setViewFromCamera(worldCamera);

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
                auto vertices = std::array {
                    sf::Vertex { .position = joePos },
                    sf::Vertex { magnet.position * 32.f },
                };
                window.getSfmlWindowContext().draw(
                    vertices.data(), 2u, sf::PrimitiveType::Lines);
            }
        }
    }

    if (settings.renderColliders) scene.world->DebugDraw();

    window.setViewFromCamera(hudCamera);

    if (!scene.playing)
    {
        text.setString("Press SPACE to start");
        text.setPosition(
            sf::Vector2f(window.getSize()) / 2.f
            - text.getGlobalBounds().size / 2.f);
        window.draw(text);
    }
    else if (scene.contactListener->won)
    {
        text.setString("You won!");
        text.setPosition(
            sf::Vector2f(window.getSize()) / 2.f
            - text.getGlobalBounds().size / 2.f);
        window.draw(text);
    }

    text.setPosition({ 10.f, 10.f });
    text.setString(fpsCounter.getText());
    window.draw(text);
}
