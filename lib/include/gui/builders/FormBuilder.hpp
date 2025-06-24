#pragma once

#include <DGM/classes/Compatibility.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <string>
#include <tuple>
#include <vector>

// https://github.com/llvm/llvm-project/issues/36032
struct [[nodiscard]] OptionConfig final
{
    bool disabled = false;
    std::optional<std::string> tooltipText = {};
};

class [[nodiscard]] FormBuilder final
{
public:
    FormBuilder() = default;
    FormBuilder(const FormBuilder&) = delete;
    FormBuilder(FormBuilder&&) = delete;
    ~FormBuilder() = default;

public:
    FormBuilder& addOption(
        const std::string& labelText,
        tgui::Widget::Ptr widget,
        OptionConfig config = {});

    FormBuilder& addOptionWithWidgetId(
        const std::string& labelText,
        tgui::Widget::Ptr widget,
        const std::string widgetId);

    FormBuilder& addOptionWithSubmit(
        const std::string& labelText,
        tgui::Widget::Ptr widget,
        tgui::Button::Ptr submitBtn);

    FormBuilder& addSeparator();

    NODISCARD_RESULT tgui::Container::Ptr
    build(tgui::Color backgroundColor = tgui::Color::Transparent);

private:
    static NODISCARD_RESULT tgui::Panel::Ptr createOptionRow(
        const std::string& labelText,
        tgui::Widget::Ptr widgetPtr,
        std::optional<std::string> widgetId);

    static NODISCARD_RESULT tgui::Panel::Ptr createOptionRowWithSubmitButton(
        const std::string& labelText,
        tgui::Widget::Ptr widgetPtr,
        tgui::Button::Ptr buttonPtr);

private:
    struct RowProps
    {
        bool separator = false;
        std::string label;
        tgui::Widget::Ptr widget;
        std::optional<std::string> widgetId = {};
        std::optional<tgui::Button::Ptr> submitBtn = {};
        std::optional<std::string> tooltipText = {};
    };

    std::vector<RowProps> rowsToBuild;
};
