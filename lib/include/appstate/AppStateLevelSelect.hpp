#pragma once

#include "misc/DependencyContainer.hpp"
#include "settings/AppSettings.hpp"
#include <DGM/dgm.hpp>

class [[nodiscard]] AppStateLevelSelect final : public dgm::AppState
{
public:
    AppStateLevelSelect(
        dgm::App& app,
        DependencyContainer& dic,
        AppSettings& settings) noexcept;

public:
    void input() override;

    void update() override;

    void draw() override;

private:
    void restoreFocusImpl(const std::string& message = "") override;

    void onTabClicked(const tgui::String& tabName);

    void buildLayout();

    void
    buildLevelCards(size_t startIdx, size_t columnCount, size_t rowCount) const;

    void buildContentGrasslands() const;

    void buildContentFactory() const;

    tgui::Container::Ptr buildLevelCard(size_t levelIdx) const;

private:
    DependencyContainer&
        dic;               ///< Dependency container for managing dependencies
    AppSettings& settings; ///< Application settings for configuration
    std::vector<std::string> levelIds;
    tgui::Panel::Ptr content;
    tgui::String lastSelectedTab;
};
