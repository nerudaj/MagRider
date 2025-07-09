#pragma once

#include "game/TransitionAnimation.hpp"
#include "misc/DependencyContainer.hpp"
#include "settings/AppSettings.hpp"
#include <DGM/classes/AppState.hpp>

struct [[nodiscard]] EndLevelState final
{
    bool levelWon = false;
    size_t levelIdx = 0;
    float levelTime = 0.f;
};

class [[nodiscard]] AppStateLevelEndTransition final : public dgm::AppState
{
public:
    AppStateLevelEndTransition(
        dgm::App& app,
        DependencyContainer& dic,
        AppSettings& settings,
        const EndLevelState& state);

public:
    void input() override;

    void update() override;

    void draw() override;

private:
    void restoreFocusImpl(const std::string& message) override;

private:
    DependencyContainer& dic;
    AppSettings& settings;
    const EndLevelState state;
    TransitionAnimation animation;
};
