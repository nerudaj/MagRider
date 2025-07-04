#include "misc/Jukebox.hpp"

Jukebox::Jukebox(
    const dgm::ResourceManager& resmgr, const std::filesystem::path& rootDir)
    : ROOT_DIR(rootDir), playlist(resmgr.get<Playlist>("playlist.json"))
{
}

template<class T>
T getRandomArrayItem(const std::vector<T>& vec)
{
    return vec[rand() % vec.size()];
}

void Jukebox::playTitleTrack()
{
    playSong(getRandomArrayItem(playlist.menu));
}

void Jukebox::playIngameTrack()
{
    playSong(getRandomArrayItem(playlist.game));
}

std::filesystem::path Jukebox::getSongPath(const std::string& songName) const
{
    return ROOT_DIR / "music" / songName;
}

void Jukebox::playSong(const std::string& songName)
{
    std::ignore = song.openFromFile(getSongPath(songName));
    song.setLooping(true);
    song.play();
}
