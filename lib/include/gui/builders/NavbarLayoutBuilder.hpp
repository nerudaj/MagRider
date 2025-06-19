#pragma once

#include "misc/Compatibility.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <optional>

namespace priv
{
    class [[nodiscard]] FinalNavbarLayoutBuilder final
    {
    public:
        FinalNavbarLayoutBuilder(
            tgui::Widget::Ptr widget,
            tgui::Panel::Ptr content,
            const std::optional<std::string>& contentPanelId)
            : widget(widget)
            , content(content)
            , contentPanelId(contentPanelId) {};
        FinalNavbarLayoutBuilder(const FinalNavbarLayoutBuilder&) = delete;
        FinalNavbarLayoutBuilder(FinalNavbarLayoutBuilder&&) = delete;
        ~FinalNavbarLayoutBuilder() = default;

    public:
        NODISCARD_RESULT tgui::Panel::Ptr build();

    private:
        tgui::Widget::Ptr widget;
        tgui::Panel::Ptr content;
        const std::optional<std::string>& contentPanelId;
    };

    class [[nodiscard]] NavbarLayoutWithNavbarWidgetBuilder final
    {
    public:
        NavbarLayoutWithNavbarWidgetBuilder(tgui::Widget::Ptr widget)
            : widget(widget) {};
        NavbarLayoutWithNavbarWidgetBuilder(
            const NavbarLayoutWithNavbarWidgetBuilder&) = delete;
        NavbarLayoutWithNavbarWidgetBuilder(
            NavbarLayoutWithNavbarWidgetBuilder&&) = delete;
        ~NavbarLayoutWithNavbarWidgetBuilder() = default;

    public:
        FinalNavbarLayoutBuilder withContent(
            tgui::Panel::Ptr content,
            const std::optional<std::string>& contentPanelId = std::nullopt)
        {
            return FinalNavbarLayoutBuilder(widget, content, contentPanelId);
        }

    private:
        tgui::Widget::Ptr widget;
    };
} // namespace priv

class [[nodiscard]] NavbarLayoutBuilder final
{
public:
    NavbarLayoutBuilder() = default;
    NavbarLayoutBuilder(const NavbarLayoutBuilder&) = delete;
    NavbarLayoutBuilder(NavbarLayoutBuilder&&) = delete;
    ~NavbarLayoutBuilder() = default;

public:
    priv::NavbarLayoutWithNavbarWidgetBuilder
    withNavbarWidget(tgui::Widget::Ptr widget)
    {
        return priv::NavbarLayoutWithNavbarWidgetBuilder(widget);
    }
};