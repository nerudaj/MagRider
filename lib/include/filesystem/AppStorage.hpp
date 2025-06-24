#pragma once

#include <DGM/classes/Error.hpp>
#include <expected>
#include <string>

class AppStorage
{
public:
    static std::expected<std::string, dgm::Error>
    loadFile(const std::filesystem::path& file);

    static void
    saveFile(const std::filesystem::path& file, const std::string& data);
};
