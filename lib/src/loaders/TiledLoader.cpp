#include "loaders/TiledLoader.hpp"
#include <SFML/System/FileInputStream.hpp>
#include <nlohmann/json.hpp>

/// <summary>
/// Multiplatform file-loader
/// </summary>
static nlohmann::json readStreamAsJson(sf::InputStream& stream)
{
    auto size = stream.getSize();
    if (!size) throw std::runtime_error("Cannot get stream size");

    std::string buffer(*size, '\0');
    auto readBytes = stream.read(buffer.data(), *size);

    if (!readBytes)
    {
        throw std::runtime_error("Error while reading stream");
    }

    if (*readBytes != *size)
    {
        throw std::runtime_error("Could not read requested amount of bytes");
    }

    return nlohmann::json::parse(buffer);
}

tiled::FiniteMapModel TiledLoader::loadLevel(const std::filesystem::path& path)
{
    sf::FileInputStream stream(path);
    auto json = readStreamAsJson(stream);
    tiled::FiniteMapModel map = json;
    return map;
}
