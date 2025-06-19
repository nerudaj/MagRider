#pragma once

#include "enums/StringId.hpp"
#include "types/StringTypes.hpp"

using enum StringId;

const std::map<StringId, StringViewType> EN_LOCALIZATION = {
    { PlayButton, "Play" },
    { Options, "Options" },
    { PauseTitle, "Game paused" },
    { ExitButton, "Exit" },
    { Back, "Back" },

    // Options
    { VideoOptionsTab, "Video" },
    { AudioOptionsTab, "Audio" },
    { InputOptionsTab, "Input" },
    { BindingsOptionsTab, "Bindings" },
    { EnableFullscreen, "Fullscreen" },
    { SetResolution, "Resolution" },
    { SoundVolume, "Sound volume" },
    { MusicVolume, "Music volume" },
    { GamepadDeadzone, "Gamepad deadzone" },
    { CursorSpeed, "Cursor speed" },
    { BindingHeadingAction, "Action" },
    { BindingHeadingKMB, "Keyboard/Mouse" },
    { BindingsHeadingGamepad, "Gamepad" },

    // InputKind
    { InputKind_BackButton, "Back" },
    { InputKind_ConfirmButton, "Confirm" },
    { InputKind_CursorUp, "Cursor up" },
    { InputKind_CursorDown, "Cursor down" },
    { InputKind_CursorLeft, "Cursor left" },
    { InputKind_CursorRight, "Cursor right" },
    { InputKind_MenuCycleLeft, "Menu cycle left" },
    { InputKind_MenuCycleRight, "Menu cycle right" },
    { InputKind_Left, "Run left" },
    { InputKind_Right, "Run right" },
    { InputKind_Jump, "Jump" },

    // Pause
    { Resume, "Resume game" },
    { BackToMenu, "Back to main menu" },
};
