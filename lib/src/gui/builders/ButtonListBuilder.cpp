#include "gui/builders/ButtonListBuilder.hpp"
#include "gui/Sizers.hpp"
#include "gui/builders/WidgetBuilder.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <ranges>

ButtonListBuilder& ButtonListBuilder::addButton(
    const std::string& label,
    std::function<void(void)> onClick,
    const std::string& buttonId)
{
    buttonProps.emplace_back(label, onClick, buttonId);
    return *this;
}

tgui::Container::Ptr
ButtonListBuilder::build(tgui::HorizontalAlignment alignment)
{
    auto&& outerPanel = WidgetBuilder::createScrollablePanel();
    auto&& layout = tgui::GrowVerticalLayout::create();
    layout->setSize({ "50%", "100%" });

    if (alignment == tgui::HorizontalAlignment::Left)
        layout->setPosition({ "0%", "0%" });
    else if (alignment == tgui::HorizontalAlignment::Center)
        layout->setPosition({ "25%", "0%" });
    else
        layout->setPosition({ "50%", "0%" });
    layout->getRenderer()->setSpaceBetweenWidgets(Sizers::getBaseFontSize());

    outerPanel->add(layout);

    for (auto&& [idx, props] : std::views::enumerate(buttonProps))
    {
        auto&& button =
            WidgetBuilder::createMenuButton(props.label, props.onClick);

        layout->add(button, props.buttonId);
    }

    return outerPanel;
}