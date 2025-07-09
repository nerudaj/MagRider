#pragma once

#include "misc/DependencyContainer.hpp"
#include "settings/AppSettings.hpp"
#include <DGM/dgm.hpp>

class [[nodiscard]] AppStateLevelEnd : public dgm::AppState
{
public:
    AppStateLevelEnd(
        dgm::App& app,
        DependencyContainer& dic,
        AppSettings& settings,
        size_t levelIdx,
        float levelTime);

public:
    void input() override;

    void update() override;

    void draw() override;

private:
    void buildLayout(bool isNewBest, float levelTime);

    tgui::Container::Ptr buildContent(bool isNewBest, float levelTime);

private:
    DependencyContainer& dic;
    AppSettings& settings;
    sf::Time blinkTimeout = sf::Time::Zero;
};