#pragma once

#include "game/events/AudioEvents.hpp"
#include "settings/AudioSettings.hpp"
#include <DGM/classes/ResourceManager.hpp>
#include <SFML/Audio/Sound.hpp>

class AudioEngine
{
public:
    AudioEngine(
        const dgm::ResourceManager& resmgr,
        const AudioSettings& settings) noexcept
        : resmgr(resmgr), settings(settings)
    {
    }

public:
    inline void operator()(JoeMagnetizedToRedAudioEvent)
    {
        playSound("buzzRed.wav", false);
    }

    inline void operator()(JoeMagnetizedToBlueAudioEvent)
    {
        playSound("buzzBlue.wav", false);
    }

    inline void operator()(JoeWonAudioEvent)
    {
        playSound("win_fanfare.wav", true);
    }

    inline void operator()(JoeDiedAudioEvent)
    {
        playSound("lose_fanfare.wav", true);
    }

private:
    void playSound(const std::string& name, bool override);

private:
    const dgm::ResourceManager& resmgr;
    const AudioSettings& settings;
    std::optional<sf::Sound> channel = std::nullopt;
};
