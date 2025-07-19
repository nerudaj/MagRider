#include "game/engine/AudioEngine.hpp"
#include <SFML/Audio/SoundBuffer.hpp>

void AudioEngine::playSound(const std::string& name, bool override)
{
    if (channel && channel->getStatus() == sf::SoundSource::Status::Playing
        && !override)
        return;

    channel = sf::Sound(resmgr.get<sf::SoundBuffer>(name));
    channel->setVolume(settings.soundVolume);
    channel->play();
}
