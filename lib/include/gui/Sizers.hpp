#pragma once

class Sizers final
{
public:
    [[nodiscard]] static unsigned getBaseFontSize();

    [[nodiscard]] static unsigned getBaseContainerHeight();

    static void setUiScale(float scale);
};
