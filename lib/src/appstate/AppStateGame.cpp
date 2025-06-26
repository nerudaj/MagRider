#include "appstate/AppStateGame.hpp"
#include "appstate/AppStatePause.hpp"
#include "appstate/Messaging.hpp"
#include "filesystem/models/TiledModels.hpp"
#include "game/SceneBuilder.hpp"
#include "misc/Utility.hpp"
#include "types/Overloads.hpp"

static dgm::Circle createMagnetButton(const dgm::Window& window, bool left)
{
    const auto radius = window.getSize().x / 15.f;
    auto&& circle = dgm::Circle(
        { left ? radius : window.getSize().x - radius,
          window.getSize().y - radius },
        radius);
    return circle;
}

AppStateGame::AppStateGame(
    dgm::App& app,
    DependencyContainer& dic,
    AppSettings& settings,
    size_t levelIdx,
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
    , levelIdx(levelIdx)
    , pauseButton(
          { app.window.getSize().x / 20.f, app.window.getSize().x / 20.f },
          app.window.getSize().x / 20.f)
    , redButton(createMagnetButton(app.window, "left"_true))
    , blueButton(createMagnetButton(app.window, "left"_false))
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

            const auto action = [&]
            {
                if (dgm::Collision::basic(redButton, e->position))
                    return InputKind::MagnetizeRed;
                else if (dgm::Collision::basic(blueButton, e->position))
                    return InputKind::MagnetizeBlue;
                else if (dgm::Collision::basic(pauseButton, e->position))
                    return InputKind::BackButton;
                return InputKind::Jump;
            }();

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
        auto& save = settings.save;
        Utility::setBestTime(save, levelIdx, game.scene.timer);
        app.popState(Messaging::serialize<GoToNextLevel>());
    }
}

void AppStateGame::draw()
{
    game.renderingEngine.draw(app.window);

#ifdef ANDROID
    pauseButton.debugRender(app.window, sf::Color(96, 96, 96, 128));
    redButton.debugRender(app.window, sf::Color(255, 0, 0, 128));
    blueButton.debugRender(app.window, sf::Color(0, 0, 255, 128));
#endif
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
