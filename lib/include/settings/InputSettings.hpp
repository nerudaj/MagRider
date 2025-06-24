#pragma once

#include "types/SemanticTypes.hpp"
#include <nlohmann/json.hpp>

struct [[nodiscard]] InputSettings final
{
    float cursorSpeed = 500_px_per_second;
    float gamepadDeadzone = 10.f;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(InputSettings, cursorSpeed, gamepadDeadzone);
