#pragma once

#include "game/SceneBuilder.hpp"
#include "game/TiledLevel.hpp"
#include "game/engine/AudioEngine.hpp"
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
        const StringProvider& strings,
        const GameConfig& config)
        : scene(SceneBuilder::buildScene(level))
        , gameRulesEngine(gameEvents, audioEvents, scene, input, settings.input)
        , renderingEngine(
              window, resmgr, settings.video, strings, scene, level, config)
        , audioEngine(resmgr, settings.audio)
    {
    }

public:
    Scene scene;
    EventQueue<GameEvent> gameEvents;
    EventQueue<AudioEvent> audioEvents;
    GameRulesEngine gameRulesEngine;
    RenderingEngine renderingEngine;
    AudioEngine audioEngine;
};
