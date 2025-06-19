#include "gui/builders/NavbarLayoutBuilder.hpp"
#include "gui/builders/WidgetBuilder.hpp"

namespace priv
{
    tgui::Panel::Ptr FinalNavbarLayoutBuilder::build()
    {
        auto navbar = WidgetBuilder::createRow();
        navbar->add(widget);

        auto contentWrapper = WidgetBuilder::createPanel(tgui::Layout2d {
            "100%",
            ("100% - " + std::to_string(navbar->getSize().y)).c_str() });

        contentWrapper->setPosition(
            tgui::Layout2d { "0%", navbar->getSize().y });

        if (contentPanelId) content->setWidgetName(contentPanelId.value());
        contentWrapper->add(content);

        auto panel = WidgetBuilder::createPanel();
        panel->add(navbar);
        panel->add(contentWrapper);

        return panel;
    }
} // namespace priv
