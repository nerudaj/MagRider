#pragma once

#include "game/events/AudioEvents.hpp"
#include <DGM/classes/ResourceManager.hpp>
#include <SFML/Audio/Sound.hpp>

class AudioEngine
{
public:
    AudioEngine(const dgm::ResourceManager& resmgr) noexcept : resmgr(resmgr) {}

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
        playSound("fanfare.wav", true);
    }

    inline void operator()(JoeDiedAudioEvent) {}

private:
    void playSound(const std::string& name, bool override);

private:
    std::optional<sf::Sound> channel;
    const dgm::ResourceManager& resmgr;
};
