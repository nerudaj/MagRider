#pragma once

#include <string>
#include <variant>

struct [[nodiscard]] JoeMagnetizedToRedAudioEvent final
{
};

struct [[nodiscard]] JoeMagnetizedToBlueAudioEvent final
{
};

struct [[nodiscard]] JoeWonAudioEvent final
{
};

struct [[nodiscard]] JoeDiedAudioEvent final
{
};

using AudioEvent = std::variant<
    JoeMagnetizedToRedAudioEvent,
    JoeMagnetizedToBlueAudioEvent,
    JoeWonAudioEvent,
    JoeDiedAudioEvent>;
