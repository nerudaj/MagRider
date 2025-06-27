#include "appstate/AppStateGameWrapper.hpp"
#include "appstate/AppStateGame.hpp"
#include "appstate/Messaging.hpp"
#include "filesystem/models/TiledModels.hpp"

AppStateGameWrapper::AppStateGameWrapper(
    dgm::App& app,
    DependencyContainer& dic,
    AppSettings& settings,
    : dgm::AppState(app)
    const GameConfig& config) noexcept
    , dic(dic)
    , settings(settings)
    , config(config)
{
}

void AppStateGameWrapper::input()
{
    app.pushState<AppStateGame>(dic, settings, levelIdx, levelId);
}

void AppStateGameWrapper::update()
{
    // Update game logic
}

void AppStateGameWrapper::draw()
{
    // Draw the game state
}

void AppStateGameWrapper::restoreFocusImpl(const std::string& message)
{
    auto msg = Messaging::deserialize(message);
    if (msg)
    {
        std::visit(
            overloads {
                [&](RestartLevel) { /* do nothing, next input() will restart*/ },
                [&](auto) { app.popState(message); },
            },
            *msg);
    }
    else
        app.popState(message);
}
