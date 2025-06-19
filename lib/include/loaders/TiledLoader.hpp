#pragma once

#include "loaders/models/TiledModels.hpp"

class [[nodiscard]] TiledLoader final
{
public:
    static tiled::FiniteMapModel loadLevel(const std::filesystem::path& path);
};
