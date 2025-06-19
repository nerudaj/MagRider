#pragma once

#include <types/SemanticTypes.hpp>

struct [[nodiscard]] InputSettings final
{
    float cursorSpeed = 500_px_per_second;
    float gamepadDeadzone = 10.f;
};
