#include "appstate/AppStateGame.hpp"
#include "appstate/AppStatePause.hpp"
#include "appstate/Messaging.hpp"
#include "loaders/models/TiledModels.hpp"
#include "types/Overloads.hpp"

void AppStateGame::input()
{
    if (dic.input.isBackButtonPressed())
    {
        app.pushState<AppStatePause>(dic, settings);
    }

    while (const auto event = app.window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            app.exit();
        }
    }
}

void AppStateGame::update()
{
    gameRulesEngine.update(app.time);
    renderingEngine.update(app.time);

    gameEvents.processEvents(
        [&](const DummyGameEvent& e)
        {
            sound.setBuffer(dic.resmgr.get<sf::SoundBuffer>(e.soundName));
            sound.play();
        });
}

void AppStateGame::draw()
{
    renderingEngine.draw(app.window);
}

void AppStateGame::restoreFocusImpl(const std::string& msg)
{
    if (auto message = Messaging::deserialize(msg))
    {
        std::visit([&](PopIfNotMenu&) { app.popState(msg); }, *message);
    }
}

dgm::Camera createFullscreenCamera(
    const dgm::Window& window, const sf::Vector2u& desiredResolution)
{
    auto&& viewport = sf::FloatRect {
        { 0.f, 0.f },
        { 1.f, 1.f },
    };

    const auto& desiredResolutionF = sf::Vector2f(desiredResolution);
    const auto& currentResolution = sf::Vector2f(window.getSize());
    const auto& desiredAspectRatio =
        desiredResolutionF.x / desiredResolutionF.y;
    const auto& currentAspectRatio = currentResolution.x / currentResolution.y;

    if (currentAspectRatio > desiredAspectRatio)
    {
        // Narrow desired into wider current
        viewport.size.x = desiredResolutionF.x / currentResolution.x;
        viewport.position.x = (1.f - viewport.size.x) / 2.f;
    }
    else if (currentAspectRatio < desiredAspectRatio)
    {
        // Wider desired into narrower current
        viewport.size.y = desiredResolutionF.y / currentResolution.y;
        viewport.position.y = (1.f - viewport.size.y) / 2.f;
    }

    return dgm::Camera(viewport, desiredResolutionF);
}

Scene AppStateGame::buildScene(
    const dgm::ResourceManager& resmgr, const dgm::Window& window)
{
    const auto& map = resmgr.get<tiled::FiniteMapModel>("mapa1.json");
    auto spawn = sf::Vector2f {};
    auto tilemap = dgm::TileMap(
        resmgr.get<sf::Texture>("set.png"),
        resmgr.get<dgm::Clip>("set.png.clip"));

    for (auto&& layer : map.layers)
    {
        std::visit(
            overloads {
                [&](const tiled::TileLayerModel& model)
                {
                    tilemap.build(
                        { map.tilewidth, map.tileheight },
                        model.data
                            | std::views::transform([&](int tile)
                                                    { return tile == 0 ? 12 : tile - 1; })
                            | uniranges::to<std::vector>(),
                        { map.width, map.height });
                },
                [&](const tiled::ObjectGroupModel& model)
                {
                    spawn = sf::Vector2f {
                        model.objects.front().x,
                        model.objects.front().y,
                    };
                },
            },
            layer);
    }

    return Scene {
        .position = spawn,
        .tileMap = std::move(tilemap),
        .camera = createFullscreenCamera(window, { 1280u, 720u }),
    };
}
