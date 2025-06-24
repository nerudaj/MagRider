#pragma once

#include "settings/AppSettings.hpp"
#include <DGM/dgm.hpp>
#include <filesystem>

class ResourceLoader final
{
public:
    static dgm::ResourceManager
    loadResources(const std::filesystem::path& assetDir);

    static AppSettings loadSettings(const std::filesystem::path& file);
};
