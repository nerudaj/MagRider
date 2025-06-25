#pragma once

#include "input/InputKind.hpp"
#include "misc/Compatibility.hpp"
#include "settings/BindingsSettings.hpp"
#include <DGM/dgm.hpp>

class [[nodiscard]] Input final
{
public:
    Input(const BindingsSettings& settings)
        : controller(configureController(settings))
    {
    }

    Input(Input&&) = delete;
    Input(const Input&) = delete;

public:
    void updateBindings(const BindingsSettings& settings);

    /// <summary>
    /// This is used in options to make sure that a configured
    /// key for confirm/back doesn't immediately mess up with
    /// the app.
    /// </summary>
    void forceRelease(InputKind action);

    bool isMagnetizingRed() const;

    bool isMagnetizingBlue() const;

    bool shouldStart() const;

    NODISCARD_RESULT bool isMenuCycleLeftPressed() const;

    NODISCARD_RESULT bool isMenuCycleRightPressed() const;

    NODISCARD_RESULT bool isBackButtonPressed() const;

    NODISCARD_RESULT bool isConfirmPressed() const;

    NODISCARD_RESULT sf::Vector2f getCursorDelta() const;

    void toggleInput(InputKind i, bool pressed);

private:
    bool readAndRelease(InputKind i) const;

    static dgm::Controller<InputKind>
    configureController(const BindingsSettings& settings);

private:
    mutable dgm::Controller<InputKind> controller;
    bool magnetizeRedPressed = false;
    bool magnetizeBluePressed = false;
    mutable bool gameInteractPressed = false;
};
