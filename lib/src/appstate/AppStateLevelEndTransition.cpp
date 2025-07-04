#include "appstate/AppStateLevelEndTransition.hpp"
#include "appstate/Messaging.hpp"
#include "game/Constants.hpp"

AppStateLevelEndTransition::AppStateLevelEndTransition(
    dgm::App& app, bool levelWon)
    : dgm::AppState(
          app, dgm::AppStateConfig { .shouldDrawUnderlyingState = true })
    , animation(
          static_cast<float>(app.window.getSize().x),
          sf::seconds(1.f),
          levelWon ? sf::Color::White : sf::Color::Red,
          sf::Vector2f(app.window.getSize() / 2u))
    , levelWon(levelWon)
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
        if (levelWon)
            app.popState(Messaging::serialize<PopIfNotLevelSelection>());
        else
            app.popState(Messaging::serialize<RestartLevel>());
    }
}

void AppStateLevelEndTransition::draw()
{
    app.window.draw(animation.getDrawable());
}
