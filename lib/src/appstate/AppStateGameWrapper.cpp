#include "appstate/AppStateGameWrapper.hpp"

AppStateGameWrapper::AppStateGameWrapper(
    dgm::App& app,
    DependencyContainer& dic,
    AppSettings& settings) noexcept
    : dgm::AppState(app), dic(dic), settings(settings)
{
}

void AppStateGameWrapper::input()
{
    // Handle input for the game state
}

void AppStateGameWrapper::update()
{
    // Update game logic
}

void AppStateGameWrapper::draw()
{
    // Draw the game state
}

void restoreFocusImpl(const std::string& message)
{
}
