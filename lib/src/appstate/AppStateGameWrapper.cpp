#include "appstate/AppStateGameWrapper.hpp"
#include "appstate/AppStateGame.hpp"
#include "appstate/Messaging.hpp"
#include "filesystem/models/TiledModels.hpp"

AppStateGameWrapper::AppStateGameWrapper(
    dgm::App& app,
    DependencyContainer& dic,
    AppSettings& settings,
    const std::string& levelId) noexcept
    : dgm::AppState(app), dic(dic), settings(settings), levelId(levelId)
{
}

void AppStateGameWrapper::input()
{
    app.pushState<AppStateGame>(dic, settings, levelId);
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
                [&](auto) { app.popState(); },
            },
            *msg);
    }
    else
        app.popState();
}
