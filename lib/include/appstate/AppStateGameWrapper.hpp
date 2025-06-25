#pragma once

#include "misc/DependencyContainer.hpp"
#include "settings/AppSettings.hpp"
#include <DGM/classes/AppState.hpp>

class [[nodiscard]] AppStateGameWrapper final : public dgm::AppState
{
public:
    AppStateGameWrapper(dgm::App& app,
        DependencyContainer& dic,
        AppSettings& settings) noexcept;

public:
    void input() override;

    void update() override;

    void draw() override;

private:
    void restoreFocusImpl(const std::string& message = "") override;

private:
    DependencyContainer& dic; ///< Dependency container for managing dependencies
    AppSettings& settings;    ///< Application settings for configuration
};