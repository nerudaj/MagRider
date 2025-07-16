#include "misc/Utility.hpp"

std::string Utility::formatTime(const float value)
{
    const auto minutes = static_cast<int>(value / 60.f);
    float seconds = 0;
    const auto fractional =
        static_cast<int>(100.f * std::modf(value - 60.f * minutes, &seconds));
    return uni::format("{:02}:{:02.0f}.{:02d}", minutes, seconds, fractional);
}

std::optional<float>
Utility::getBestTime(SaveState& save, const size_t levelIdx)
{
    if (save.times.size() <= levelIdx)
    {
        save.times.resize(levelIdx + 1);
    }

    return save.times[levelIdx] == 0.f
               ? std::nullopt
               : std::make_optional(save.times[levelIdx]);
}

bool Utility::setBestTime(
    SaveState& save, const size_t levelIdx, const float time)
{
    if (save.times.size() <= levelIdx)
    {
        save.times.resize(levelIdx + 1);
    }

    if (save.times[levelIdx] == 0.f || save.times[levelIdx] > time)
    {
        save.times[levelIdx] = time;
        return true;
    }

    return false;
}
