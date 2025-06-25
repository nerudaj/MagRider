#include "appstate/AppStateGameWrapper.hpp"
#include "appstate/AppStateGame.hpp"
#include "appstate/Messaging.hpp"
#include "filesystem/models/TiledModels.hpp"

AppStateGameWrapper::AppStateGameWrapper(
    dgm::App& app, DependencyContainer& dic, AppSettings& settings) noexcept
    : dgm::AppState(app)
    , dic(dic)
    , settings(settings)
    , levels(dic.resmgr.getLoadedResourceIds<tiled::FiniteMapModel>().value())
{
}

void AppStateGameWrapper::input()
{
    app.pushState<AppStateGame>(dic, settings, levels[currentLevelIdx]);
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
    if (!msg)
    {
        app.popState();
    }

    std::visit(
        overloads {
            [&](RestartLevel) { /* do nothing, next input() will restart*/ },
            [&](GoToNextLevel)
            { currentLevelIdx = (currentLevelIdx + 1) % levels.size(); },
            [&](PopIfNotMenu) { app.popState(); },
        },
        *msg);
}
