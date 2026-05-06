#pragma once

#include "misc/DependencyContainer.hpp"
#include "settings/AppSettings.hpp"
#include <DGM/classes/AppState.hpp>

class [[nodiscard]] AppStateHint : public dgm::AppState
{
public:
    AppStateHint(
        dgm::App& app,
        DependencyContainer& dic,
        AppSettings& settings,
        const std::string& message)
        : dgm::AppState(
              app,
              dgm::AppStateConfig {
                  .shouldDrawUnderlyingState = true,
              })
        , dic(dic)
        , settings(settings)
        , message(message)
    {
        buildLayout();
    }

public:
    void input() override;

    void update() override;

    void draw() override;

private:
    void buildLayout();

    tgui::Container::Ptr buildHintCard();

    void onContinue();

private:
    DependencyContainer& dic;
    AppSettings& settings;
    std::string message;
};
