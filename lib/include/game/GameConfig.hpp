#pragma once

#include <cstdint>
#include <string>

struct [[nodiscard]] GameConfig final
{
    size_t levelIdx;
    std::string levelResourceName;
};
