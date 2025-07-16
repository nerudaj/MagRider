#include "appstate/AppStateLevelEndTransition.hpp"
#include "appstate/AppStateLevelEnd.hpp"
#include "appstate/Messaging.hpp"
#include "game/Constants.hpp"

AppStateLevelEndTransition::AppStateLevelEndTransition(
    dgm::App& app,
    DependencyContainer& dic,
    AppSettings& settings,
    const EndLevelState& state)
    : dgm::AppState(
          app, dgm::AppStateConfig { .shouldDrawUnderlyingState = true })
    , dic(dic)
    , settings(settings)
    , state(state)
    , animation(
          static_cast<float>(app.window.getSize().x),
          sf::seconds(1.f),
          state.levelWon ? COLOR_WHITE : COLOR_RED,
          sf::Vector2f(app.window.getSize() / 2u))
{
}

void AppStateLevelEndTransition::input()
{
    while (const auto event = app.window.pollEvent())
    {
        if (event->is<sf::Event::Closed>()) app.exit();
    }
}

void AppStateLevelEndTransition::update()
{
    animation.update(app.time.getElapsed());

    if (animation.isFinished())
    {
        if (state.levelWon)
            app.pushState<AppStateLevelEnd>(
                dic, settings, state.levelIdx, state.levelTime);
        else
            app.popState(Messaging::serialize<RestartLevel>());
    }
}

void AppStateLevelEndTransition::draw()
{
    app.window.draw(animation.getDrawable());
}

void AppStateLevelEndTransition::restoreFocusImpl(const std::string& message)
{
    app.popState(message);
}
