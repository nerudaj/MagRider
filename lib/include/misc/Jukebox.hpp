#include "misc/Playlist.hpp"
#include <DGM/classes/ResourceManager.hpp>
#include <SFML/Audio/Music.hpp>

class [[nodiscard]] Jukebox final
{
public:
    Jukebox(
        const dgm::ResourceManager& resmgr,
        const std::filesystem::path& rootDir);

public:
    void playTitleTrack();

    void playIngameTrack();

    void pause()
    {
        song.pause();
    }

    void resume()
    {
        song.play();
    }

    void setVolume(float volume)
    {
        song.setVolume(volume);
    }

private:
    std::filesystem::path getSongPath(const std::string& songName) const;

    void playSong(const std::string& songName);

private:
    const std::filesystem::path ROOT_DIR;
    Playlist playlist;
    sf::Music song;
};
