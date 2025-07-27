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
    { LevelFinished, "level finished" },
    { Continue, "continue" },
    { Retry, "retry" },

    // Select
    { Grasslands, "grasslands" },
    { Factory, "factory" },

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
    { ShowFPS, "show fps" },
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
    { BackToLevelSelect, "level selection" },

    // Game
    { TouchToStart, "touch screen to start" },
    { SpaceToStart, "press space to start" },
    { Tutorial1, "   magnetize red to\nrepel from red magnets" },
    { Tutorial2, "maintain initial velocity" },
    { Tutorial3, "avoid spikes!" },
#ifdef ANDROID
    { Tutorial4, " touch colored circles to\nto magnetize to given color" },
#else
    { Tutorial4, "press a to magnetize red\npress d to magnetize blue" },
#endif
    { Tutorial5, "change polarity mid-flight\n    to propel yourself" },

    // End Screen
    { YourTime, "your time" },
    { NewBest, "new best" },
};
