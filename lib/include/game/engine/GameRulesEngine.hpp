#pragma once

#include "game/Scene.hpp"
#include "game/events/AudioEvents.hpp"
#include "game/events/EventQueue.hpp"
#include "game/events/GameEvents.hpp"
#include "input/Input.hpp"

class [[nodiscard]] GameRulesEngine final
{
public:
    GameRulesEngine(
        EventQueue<GameEvent>& gameEventQueue,
        EventQueue<AudioEvent>& audioEventQueue,
        Scene& scene,
        Input& input) noexcept
        : gameEventQueue(gameEventQueue)
        , audioEventQueue(audioEventQueue)
        , scene(scene)
        , input(input)
    {
    }

    GameRulesEngine(GameRulesEngine&&) = delete;
    GameRulesEngine(const GameRulesEngine&) = delete;

public:
    void update(const dgm::Time& time);

private:
    EventQueue<GameEvent>& gameEventQueue;
    EventQueue<AudioEvent>& audioEventQueue;
    Scene& scene;
    Input& input;
};