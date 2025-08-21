#pragma once

// localizations can be found under private_include/localizations

enum class [[nodiscard]] StringId
{
    PlayButton,
    Options,
    PauseTitle,
    ExitButton,
    Back,
    SelectLevel,
    LevelFinished,
    Continue,
    Retry,
    Apply,

    // Select
    Grasslands,
    Factory,
    Locked,

    // Options
    VideoOptionsTab,
    AudioOptionsTab,
    InputOptionsTab,
    BindingsOptionsTab,
    EnableFullscreen,
    SetResolution,
    SetUiScale,
    SetTheme,
    RenderColliders,
    ShowFPS,
    SoundVolume,
    MusicVolume,
    GamepadDeadzone,
    CursorSpeed,
    BindingHeadingAction,
    BindingHeadingKMB,
    BindingsHeadingGamepad,
    SameColorAttracts,

    // InputKind
    InputKind_BackButton,
    InputKind_ConfirmButton,
    InputKind_CursorUp,
    InputKind_CursorDown,
    InputKind_CursorLeft,
    InputKind_CursorRight,
    InputKind_MenuCycleLeft,
    InputKind_MenuCycleRight,
    InputKind_Left,
    InputKind_Right,
    InputKind_Jump,

    // Pause
    Resume,
    Restart,
    BackToLevelSelect,

    // Game
    TouchToStart,
    SpaceToStart,
    Tutorial1,
    Tutorial2,
    Tutorial3,
    Tutorial4,
    Tutorial5,
    Tutorial6,

    // End Screen
    YourTime,
    NewBest,

    MaxId, // Make sure this one is always the last!
};
