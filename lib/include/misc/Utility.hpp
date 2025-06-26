#pragma once

#include "misc/Compatibility.hpp"
#include <settings/SaveState.hpp>

class [[nodiscard]] Utility final
{
public:
    static std::string formatTime(const float value)
    {
        const auto minutes = value / 60.f;
        return uni::format("{:02.0f}:{:02.2f}", minutes, value - minutes);
    }

    static std::optional<float>
    getBestTime(SaveState& save, const size_t levelIdx)
    {
        if (save.times.size() <= levelIdx)
        {
            save.times.resize(levelIdx + 1);
        }

        return save.times[levelIdx] == 0.f
                   ? std::nullopt
                   : std::make_optional(save.times[levelIdx]);
    }

    static void
    setBestTime(SaveState& save, const size_t levelIdx, const float time)
    {
        if (save.times.size() <= levelIdx)
        {
            save.times.resize(levelIdx + 1);
        }

        if (save.times[levelIdx] == 0.f || save.times[levelIdx] > time)
            save.times[levelIdx] = time;
    }
};
