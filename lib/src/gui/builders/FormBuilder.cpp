#include "gui/builders/FormBuilder.hpp"
#include "gui/Sizers.hpp"
#include "gui/builders/WidgetBuilder.hpp"
#include <ranges>

FormBuilder& FormBuilder::addOption(
    const std::string& labelText, tgui::Widget::Ptr widget, OptionConfig config)
{
    widget->setEnabled(!config.disabled);
    rowsToBuild.push_back({ .label = labelText,
                            .widget = widget,
                            .tooltipText = config.tooltipText });
    return *this;
}

FormBuilder& FormBuilder::addOptionWithWidgetId(
    const std::string& labelText,
    tgui::Widget::Ptr widget,
    const std::string widgetId)
{
    rowsToBuild.push_back(
        { .label = labelText, .widget = widget, .widgetId = widgetId });
    return *this;
}

FormBuilder& FormBuilder::addOptionWithSubmit(
    const std::string& labelText,
    tgui::Widget::Ptr widget,
    tgui::Button::Ptr submitBtn)
{
    rowsToBuild.push_back(
        { .label = labelText, .widget = widget, .submitBtn = submitBtn });
    return *this;
}

FormBuilder& FormBuilder::addSeparator()
{
    rowsToBuild.push_back({ .separator = true });
    return *this;
}

tgui::Container::Ptr FormBuilder::build(tgui::Color backgroundColor)
{
    auto&& verticalLayout = tgui::GrowVerticalLayout::create();
    verticalLayout->getRenderer()->setPadding({ 10.f, 10.f });

    for (auto&& [idx, props] : std::views::enumerate(rowsToBuild))
    {
        auto&& row =
            props.separator ? WidgetBuilder::createRow()
            : props.submitBtn
                ? createOptionRowWithSubmitButton(
                      props.label, props.widget, props.submitBtn.value())
                : createOptionRow(props.label, props.widget, props.widgetId);

        if (props.separator)
        {
            row->add(WidgetBuilder::createSeparator());
        }
        else if (props.tooltipText.has_value())
        {
            row->setToolTip(
                WidgetBuilder::createTooltip(props.tooltipText.value()));
        }

        verticalLayout->add(row);
    }

    return verticalLayout;
}

tgui::Panel::Ptr FormBuilder::createOptionRow(
    const std::string& labelText,
    tgui::Widget::Ptr widgetPtr,
    std::optional<std::string> widgetId)
{
    auto&& row = WidgetBuilder::createRow();
    row->add(WidgetBuilder::createTextLabel(labelText));

    auto&& widgetPanel = tgui::Panel::create({ "40%", "100%" });
    widgetPanel->setPosition("60%", "0%");
    widgetPanel->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
    row->add(widgetPanel);

    widgetId
        .or_else(
            [&]() -> std::optional<std::string>
            {
                widgetPanel->add(widgetPtr);
                return std::nullopt;
            })
        .and_then(
            [&](auto id) -> std::optional<std::string>
            {
                widgetPanel->add(widgetPtr, id);
                return id;
            });

    return row;
}

tgui::Panel::Ptr FormBuilder::createOptionRowWithSubmitButton(
    const std::string& labelText,
    tgui::Widget::Ptr widgetPtr,
    tgui::Button::Ptr buttonPtr)
{
    auto&& row = WidgetBuilder::createRow();
    row->add(WidgetBuilder::createTextLabel(labelText));

    auto&& widgetPanel = tgui::Panel::create({ "25%", "100%" });
    widgetPanel->setPosition("60%", "0%");
    widgetPanel->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
    widgetPanel->add(widgetPtr);
    row->add(widgetPanel);

    auto&& btnPanel = tgui::Panel::create({ "15%", "100%" });
    btnPanel->setPosition("85%", "0%");
    btnPanel->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
    btnPanel->add(buttonPtr);
    row->add(btnPanel);

    return row;
}