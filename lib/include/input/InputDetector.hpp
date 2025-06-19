#pragma once

#include "types/Binding.hpp"
#include <functional>

class [[nodiscard]] InputDetector final
{
public:
    using ReportKmbCallbackType = std::function<void(KmbBinding)>;
    using ReportGamepadCallbackType = std::function<void(GamepadBinding)>;
    using CancelCallbackType = std::function<void()>;

    void startCheckingInputs(
        ReportKmbCallbackType reportCallback, CancelCallbackType cancelCallback)
    {
        reportKmb = reportCallback;
        cancel = cancelCallback;
        runMode = RunMode::DetectingKmb;
        initialDelay = sf::seconds(1.f).asSeconds();
    }

    void startCheckingInputs(
        ReportGamepadCallbackType reportCallback,
        CancelCallbackType cancelCallback)
    {
        reportGamepad = reportCallback;
        cancel = cancelCallback;
        runMode = RunMode::DetectingGamepad;
        initialDelay = sf::seconds(1.f).asSeconds();
    }

    void update(const dgm::Time& time);

    NODISCARD_RESULT bool isDetectionInProgress() const noexcept
    {
        return runMode != RunMode::Idle;
    }

private:
    enum class [[nodiscard]] RunMode
    {
        Idle,
        DetectingKmb,
        DetectingGamepad,
    };

    enum class [[nodiscard]] DetectionStatus
    {
        None,
        InputDetected
    };

    DetectionStatus tryGamepadAxis();

    DetectionStatus tryGamepadButton();

    DetectionStatus tryKeyboard();

    DetectionStatus tryMouse();

private:
    RunMode runMode = RunMode::Idle;
    ReportKmbCallbackType reportKmb;
    ReportGamepadCallbackType reportGamepad;
    CancelCallbackType cancel;
    float initialDelay = 0;
};