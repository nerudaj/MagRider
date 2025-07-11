#pragma once

#include "misc/Compatibility.hpp"
#include <settings/SaveState.hpp>

class [[nodiscard]] Utility final
{
public:
    static std::string formatTime(const float value);

    static std::optional<float>
    getBestTime(SaveState& save, const size_t levelIdx);

    /// <summary>
    /// Returns true when new best time was set
    /// </summary>
    static bool
    setBestTime(SaveState& save, const size_t levelIdx, const float time);
};
