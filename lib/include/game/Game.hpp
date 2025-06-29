#pragma once

#include "game/SceneBuilder.hpp"
#include "game/TiledLevel.hpp"
#include "game/engine/GameRulesEngine.hpp"
#include "game/engine/RenderingEngine.hpp"
#include "game/events/EventQueue.hpp"
#include "input/Input.hpp"
#include "settings/AppSettings.hpp"
#include <DGM/classes/ResourceManager.hpp>

class [[nodiscard]] Game final
{
public:
    Game(
        const TiledLevel& level,
        Input& input,
        dgm::Window& window,
        dgm::ResourceManager& resmgr,
        const AppSettings& settings,
        const StringProvider& strings)
        : scene(SceneBuilder::buildScene(level))
        , gameRulesEngine(gameEvents, scene, input)
        , renderingEngine(window, resmgr, settings.video, strings, scene, level)
    {
    }

public:
    Scene scene;
    EventQueue<GameEvent> gameEvents;
    GameRulesEngine gameRulesEngine;
    RenderingEngine renderingEngine;
};
