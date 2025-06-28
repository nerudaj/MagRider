#pragma once

#include "strings/StringId.hpp"
#include "types/StringTypes.hpp"

using enum StringId;

const std::map<StringId, StringViewType> EN_LOCALIZATION = {
    { PlayButton, "play" },
    { Options, "options" },
    { PauseTitle, "game paused" },
    { ExitButton, "exit" },
    { Back, "back" },
    { SelectLevel, "select level" },

    // Options
    { VideoOptionsTab, "video" },
    { AudioOptionsTab, "audio" },
    { InputOptionsTab, "input" },
    { BindingsOptionsTab, "bindings" },
    { EnableFullscreen, "fullscreen" },
    { SetResolution, "resolution" },
    { SetUiScale, "set ui scale" },
    { SetTheme, "set theme" },
    { RenderColliders, "render box2d colliders" },
    { SoundVolume, "sound volume" },
    { MusicVolume, "music volume" },
    { GamepadDeadzone, "gamepad deadzone" },
    { CursorSpeed, "cursor speed" },
    { BindingHeadingAction, "action" },
    { BindingHeadingKMB, "keyboard/mouse" },
    { BindingsHeadingGamepad, "gamepad" },

    // InputKind
    { InputKind_BackButton, "back" },
    { InputKind_ConfirmButton, "confirm" },
    { InputKind_CursorUp, "cursor up" },
    { InputKind_CursorDown, "cursor down" },
    { InputKind_CursorLeft, "cursor left" },
    { InputKind_CursorRight, "cursor right" },
    { InputKind_MenuCycleLeft, "menu cycle left" },
    { InputKind_MenuCycleRight, "menu cycle right" },
    { InputKind_Left, "run left" },
    { InputKind_Right, "run right" },
    { InputKind_Jump, "jump" },

    // Pause
    { Resume, "resume game" },
    { Restart, "restart level" },
    { BackToLevelSelect, "back to level selection" },
};
