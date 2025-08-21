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
    , touchControls(dic.resmgr, dic.input, settings.input, app.window.getSize())
    , game(
          SceneBuilder::convertToTiledLevel(
              dic.resmgr.get<tiled::FiniteMapModel>(config.levelResourceName)),
          dic.input,
          app.window,
          dic.resmgr,
          settings,
          dic.strings,
          config)
{
    dic.input.reset();
}

void AppStateGame::input()
{
    if (dic.input.isBackButtonPressed())
    {
        paused = true;
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

#ifdef _DEBUG
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
    {
        static int cnt = 0;
        std::ignore = app.window.getScreenshot().saveToFile(
            uni::format("screen{}.png", cnt++));
    }
#endif
}

void AppStateGame::update()
{
    game.gameRulesEngine.update(app.time);
    game.renderingEngine.update(app.time);

    if (game.scene.contactListener->won)
    {
        game.audioEvents.pushEvent<JoeWonAudioEvent>();
    }
    else if (game.scene.contactListener->died)
    {
        game.audioEvents.pushEvent<JoeDiedAudioEvent>();
    }

    game.audioEvents.processEvents(game.audioEngine);

    if (game.scene.contactListener->died)
    {
        app.pushState<AppStateLevelEndTransition>(
            dic,
            settings,
            EndLevelState {
                .levelWon = false,
            });
    }
    else if (game.scene.contactListener->won)
    {
        app.pushState<AppStateLevelEndTransition>(
            dic,
            settings,
            EndLevelState {
                .levelWon = true,
                .levelIdx = config.levelIdx,
                .levelTime = game.scene.timer,
            });
    }
}

void AppStateGame::draw()
{
    game.renderingEngine.draw(paused);
    if (!paused) touchControls.draw(app.window);
}

void AppStateGame::restoreFocusImpl(const std::string& msg)
{
    paused = false;
    // Empty message means returning from pause menu
    if (!msg.empty())
        app.popState(msg);
    else
        // Settings might have changed touch button scaling
        touchControls.regenerateButtons(app.window.getSize(), settings.input);
}
