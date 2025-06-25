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
          dic.resmgr,
          settings)
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
        else if (event->is<sf::Event::TouchBegan>())
        {
            auto e = event->getIf<sf::Event::TouchBegan>();
            auto position = e->position;

            const auto action =
                position.x < app.window.getSize().x / 3.f
                    ? InputKind::MagnetizeRed
                : position.x < 2.f * app.window.getSize().x / 3.f
                    ? InputKind::Jump
                    : InputKind ::MagnetizeBlue;
            fingerToAction[e->finger] = action;

            dic.input.toggleInput(action, true);
        }
        else if (event->is<sf::Event::TouchEnded>())
        {
            auto e = event->getIf<sf::Event::TouchEnded>();
            dic.input.toggleInput(fingerToAction[e->finger], false);
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
        app.popState(Messaging::serialize<RestartLevel>());
    }
    else if (game.scene.contactListener->won)
    {
        app.popState(Messaging::serialize<GoToNextLevel>());
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
        std::visit(
            overloads {
                [&](PopIfNotMenu&) { app.popState(msg); },
                [](RestartLevel) {},
                [](GoToNextLevel) {},
            },
            *message);
    }
}
