#include "appstate/AppStateGame.hpp"
#include "appstate/AppStatePause.hpp"
#include "appstate/Messaging.hpp"
#include "filesystem/models/TiledModels.hpp"
#include "game/SceneBuilder.hpp"
#include "types/Overloads.hpp"

AppStateGame::AppStateGame(
    dgm::App& app,
    DependencyContainer& dic,
    AppSettings& settings,
    const std::string& levelName)
    : dgm::AppState(app)
    , dic(dic)
    , settings(settings)
    , game(
          SceneBuilder::convertToTiledLevel(
              dic.resmgr.get<tiled::FiniteMapModel>(levelName)),
          dic.input,
          app.window,
          dic.resmgr)
    , sound(dic.resmgr.get<sf::SoundBuffer>("land.wav"))
{
    sound.setVolume(100.f);
}

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
    game.gameRulesEngine.update(app.time);
    game.renderingEngine.update(app.time);

    game.gameEvents.processEvents(
        [&](const DummyGameEvent& e)
        {
            sound.setBuffer(dic.resmgr.get<sf::SoundBuffer>(e.soundName));
            sound.play();
        });

    if (game.scene.contactListener->died)
    {
        app.popState();
    }
}

void AppStateGame::draw()
{
    game.renderingEngine.draw(app.window);
}

void AppStateGame::restoreFocusImpl(const std::string& msg)
{
    if (auto message = Messaging::deserialize(msg))
    {
        std::visit([&](PopIfNotMenu&) { app.popState(msg); }, *message);
    }
}

/*
Scene AppStateGame::buildScene(
    const dgm::ResourceManager& resmgr, const dgm::Window& window)
{
    const auto& map = resmgr.get<tiled::FiniteMapModel>("intro.json");
    auto spawn = sf::Vector2f {};
    auto tilemap = ;

    return SceneBuilder::buildScene(SceneBuilder::convertToTiledLevel(map));
}

*/