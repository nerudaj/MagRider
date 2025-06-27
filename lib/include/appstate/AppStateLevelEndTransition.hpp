#pragma once

#include "game/TransitionAnimation.hpp"
#include "misc/DependencyContainer.hpp"
#include "settings/AppSettings.hpp"
#include <DGM/classes/AppState.hpp>

class [[nodiscard]] AppStateLevelEndTransition final : public dgm::AppState
{
public:
    AppStateLevelEndTransition(dgm::App& app, bool levelWon);

public:
    void input() override;

    void update() override;

    void draw() override;

private:
    TransitionAnimation animation;
    bool levelWon;
};
