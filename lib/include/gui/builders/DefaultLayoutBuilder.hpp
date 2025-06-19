#pragma once

#include "gui/HeadingLevel.hpp"
#include "misc/Compatibility.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

namespace priv
{
    class [[nodiscard]] FinalizedLayoutBuilder final
    {
    public:
        FinalizedLayoutBuilder(tgui::Panel::Ptr container)
            : container(container)
        {
        }

    public:
        NODISCARD_RESULT inline tgui::Panel::Ptr build() const
        {
            return container;
        }

    private:
        tgui::Panel::Ptr container;
    };

    class [[nodiscard]] LayoutBuilderWithContentAndBackButton final
    {
    public:
        LayoutBuilderWithContentAndBackButton(tgui::Panel::Ptr container)
            : container(container)
        {
        }

    public:
        FinalizedLayoutBuilder withNoSubmitButton() const
        {
            return FinalizedLayoutBuilder(container);
        }

        FinalizedLayoutBuilder withSubmitButton(tgui::Button::Ptr button);

    private:
        tgui::Panel::Ptr container;
    };

    class [[nodiscard]] LayoutBuilderWithContent final
    {
    public:
        LayoutBuilderWithContent(tgui::Panel::Ptr container)
            : container(container)
        {
        }

    public:
        LayoutBuilderWithContentAndBackButton withNoBackButton() const
        {
            return LayoutBuilderWithContentAndBackButton(container);
        }

        LayoutBuilderWithContentAndBackButton
        withBackButton(tgui::Button::Ptr button);

    private:
        tgui::Panel::Ptr container;
    };

    class [[nodiscard]] LayoutBuilderWithBackgroundAndTitle final
    {
    public:
        LayoutBuilderWithBackgroundAndTitle(tgui::Panel::Ptr container)
            : container(container)
        {
        }

    public:
        LayoutBuilderWithContent withContent(tgui::Container::Ptr content);

    private:
        tgui::Panel::Ptr container;
    };

    class [[nodiscard]] LayoutBuilderWithBackground final
    {
    public:
        LayoutBuilderWithBackground(tgui::Panel::Ptr container)
            : container(container)
        {
        }

    public:
        LayoutBuilderWithBackgroundAndTitle
        withTitle(const std::string& title, HeadingLevel level);

    private:
        tgui::Panel::Ptr container;
    };
} // namespace priv

class [[nodiscard]] DefaultLayoutBuilder final
{
public:
    static priv::LayoutBuilderWithBackground
    withBackgroundImage(const sf::Texture& texture);

    static priv::LayoutBuilderWithBackground withNoBackgroundImage();
};