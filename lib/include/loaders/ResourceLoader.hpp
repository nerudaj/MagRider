#pragma once

#include <DGM/dgm.hpp>

#include <filesystem>

class ResourceLoader final
{
public:
    static dgm::ResourceManager loadResources(const std::filesystem::path& assetDir);
};
