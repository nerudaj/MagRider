#include "appstate/AppStateGame.hpp"
#include "appstate/AppStateLevelEndTransition.hpp"
#include "appstate/AppStatePause.hpp"
#include "appstate/Messaging.hpp"
#include "filesystem/models/TiledModels.hpp"
#include "game/SceneBuilder.hpp"
#include "misc/Utility.hpp"
#include "types/Overloads.hpp"

AppStateGame::AppStateGame(
    dgm::App& app,
    DependencyContainer& dic,
    AppSettings& settings,
    const GameConfig& config)
    : dgm::AppState(app)
    , dic(dic)
    , settings(settings)
    , config(config)
    , touchControls(dic.input, app.window.getSize())
    , game(
          SceneBuilder::convertToTiledLevel(
              dic.resmgr.get<tiled::FiniteMapModel>(config.levelResourceName)),
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
            touchControls.processEvent(*event->getIf<sf::Event::TouchBegan>());
        }
        else if (event->is<sf::Event::TouchEnded>())
        {
            touchControls.processEvent(*event->getIf<sf::Event::TouchEnded>());
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
        app.pushState<AppStateLevelEndTransition>("levelWon"_false);
    }
    else if (game.scene.contactListener->won)
    {
        auto& save = settings.save;
        Utility::setBestTime(save, config.levelIdx, game.scene.timer);
        app.pushState<AppStateLevelEndTransition>("levelWon"_true);
    }
}

void AppStateGame::draw()
{
    game.renderingEngine.draw(app.window);
    touchControls.draw(app.window);
}

void AppStateGame::restoreFocusImpl(const std::string& msg)
{
    app.popState(msg);
}
