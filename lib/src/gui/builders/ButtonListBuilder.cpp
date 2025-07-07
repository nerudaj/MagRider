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
    layout->setWidth("50%");

    if (alignment == tgui::HorizontalAlignment::Left)
        layout->setPosition({ "0%", "parent.height - height" });
    else if (alignment == tgui::HorizontalAlignment::Center)
        layout->setPosition({ "25%", "parent.height - height" });
    else
        layout->setPosition({ "50%", "parent.height - height" });

    outerPanel->add(layout);

    for (auto&& [idx, props] : std::views::enumerate(buttonProps))
    {
        auto&& button =
            WidgetBuilder::createMenuButton(props.label, props.onClick);

        layout->add(button, props.buttonId);
    }

#ifndef ANDROID
    layout->getRenderer()->setSpaceBetweenWidgets(Sizers::getBaseFontSize());
#endif

    return outerPanel;
}