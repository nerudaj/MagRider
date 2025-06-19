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

tgui::Panel::Ptr ButtonListBuilder::build()
{
    auto&& outerPanel = WidgetBuilder::createPanel();

    auto&& buttonListHeight =
        buttonProps.size() * Sizers::getBaseContainerHeight() * 2.1f;
    auto&& panel = WidgetBuilder::createPanel({ "60%", buttonListHeight });
    panel->setPosition(
        { "20%",
          ("parent.height - " + std::to_string(buttonListHeight)).c_str() });
    outerPanel->add(panel);

    for (auto&& [idx, props] : std::views::enumerate(buttonProps))
    {
        auto&& button = WidgetBuilder::createButton(props.label, props.onClick);
        button->setSize({ "100%", Sizers::getBaseContainerHeight() * 1.5f });
        button->setTextSize(Sizers::getBaseFontSize() * 2u);
        button->setPosition(
            { "0%", Sizers::getBaseContainerHeight() * idx * 2.1f });

        if (props.buttonId.empty())
            panel->add(button);
        else
            panel->add(button, props.buttonId);
    }

    return outerPanel;
}