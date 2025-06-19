#pragma once

#include "settings/AudioSettings.hpp"
#include "settings/BindingsSettings.hpp"
#include "settings/InputSettings.hpp"
#include "settings/VideoSettings.hpp"

struct [[nodiscard]] AppSettings final
{
    AudioSettings audio;
    VideoSettings video;
    InputSettings input;
    BindingsSettings bindings;
};
