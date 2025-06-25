#pragma once

#include "game/Game.hpp"
#include "game/Scene.hpp"
#include "game/engine/GameRulesEngine.hpp"
#include "game/engine/RenderingEngine.hpp"
#include "game/events/EventQueue.hpp"
#include "misc/DependencyContainer.hpp"
#include "settings/AppSettings.hpp"
#include <DGM/dgm.hpp>
#include <SFML/Audio.hpp>
#include <vector>

class [[nodiscard]] AppStateGame : public dgm::AppState
{
public:
    AppStateGame(
        dgm::App& app,
        DependencyContainer& dic,
        AppSettings& settings,
        const std::string& levelName);

public:
    void input() override;

    void update() override;

    void draw() override;

private:
    void restoreFocusImpl(const std::string& msg) override;

    static Scene
    buildScene(const dgm::ResourceManager& resmgr, const dgm::Window& window);

private:
    DependencyContainer& dic;
    AppSettings& settings;
    Game game;
    sf::Sound sound;
};
