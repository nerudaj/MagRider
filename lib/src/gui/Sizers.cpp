#include "gui/Sizers.hpp"

constexpr float CONTAINER_PADDING_MULTIPLIER = 2.55f;
static inline float UI_SCALE = 1.f;

#ifdef ANDROID

#include <jni/Jni.hpp>

constexpr float ANDROID_BASE_UNSCALED_FONT_SIZE = 14;

/**
 * Lazy-initialized base-size provider for Android.
 */
struct [[nodiscard]] BaseSizeProviderSingleton final
{
private:
    BaseSizeProviderSingleton()
    {
        auto core = jni::Core::attachCurrentThread();
        auto resources = jni::Resources::getSystem(core.getEnv());
        pixelDensity = resources.getDisplayMetrics().getDensity();
    }

public:
    BaseSizeProviderSingleton(BaseSizeProviderSingleton&&) = delete;
    BaseSizeProviderSingleton(const BaseSizeProviderSingleton&) = delete;

public:
    static BaseSizeProviderSingleton& getInstance()
    {
        static BaseSizeProviderSingleton instance;
        return instance;
    }

    [[nodiscard]] unsigned getBaseFontSize() const noexcept
    {
        return spToPx(ANDROID_BASE_UNSCALED_FONT_SIZE, pixelDensity);
    }

    [[nodiscard]] unsigned getBaseContainerHeight() const noexcept
    {
        return spToPx(
            static_cast<unsigned>(
                ANDROID_BASE_UNSCALED_FONT_SIZE * CONTAINER_PADDING_MULTIPLIER),
            pixelDensity);
    }

private:
    static unsigned spToPx(unsigned sp, float density)
    {
        return static_cast<unsigned>(sp * density * UI_SCALE);
    }

private:
    float pixelDensity = 0.f;
};

unsigned Sizers::getBaseContainerHeight()
{
    return BaseSizeProviderSingleton::getInstance().getBaseContainerHeight();
}

unsigned Sizers::getBaseFontSize()
{
    return BaseSizeProviderSingleton::getInstance().getBaseFontSize();
}
#else
#include <Windows.h>

unsigned Sizers::getBaseContainerHeight()
{
    const unsigned dpi = GetDpiForSystem();
    return static_cast<unsigned>(
        (GetSystemMetricsForDpi(SM_CYCAPTION, dpi)
         + GetSystemMetricsForDpi(SM_CYSIZEFRAME, dpi)
         + GetSystemMetricsForDpi(SM_CYEDGE, dpi) * 2)
        * UI_SCALE);
}

unsigned Sizers::getBaseFontSize()
{
    return static_cast<unsigned>(
        getBaseContainerHeight() / CONTAINER_PADDING_MULTIPLIER * UI_SCALE);
}
#endif

void Sizers::setUiScale(float scale)
{
    UI_SCALE = scale;
}
