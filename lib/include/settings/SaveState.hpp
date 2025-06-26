#pragma once

#include <nlohmann/json.hpp>
#include <optional>
#include <vector>

struct SaveState final
{
    std::vector<float> times;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SaveState, times);