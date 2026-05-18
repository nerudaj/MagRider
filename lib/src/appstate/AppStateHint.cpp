#include "appstate/AppStateHint.hpp"
#include "appstate/CommonHandler.hpp"
#include "game/Constants.hpp"
#include "gui/Builders.hpp"

void AppStateHint::input()
{
    CommonHandler::handleInput(
        app,
        dic,
        settings.input,
        CommonHandlerOptions {
            .disableGoBack = true,
        });
}

void AppStateHint::update() {}

void AppStateHint::draw()
{
    dic.gui.draw();
    dic.virtualCursor.draw();
}

void AppStateHint::buildLayout()
{
    auto layout = tgui::Group::create({ "50%", "50%" });
    layout->setPosition({ "50% - width / 2", "50% - height / 2" });

    auto panel =
        WidgetBuilder::createPanel({ "100%", "100%" }, COLOR_DARK_GREY);
    panel->getRenderer()->setBorderColor(COLOR_WHITE);
    panel->getRenderer()->setBorders({ 2 });
    layout->add(panel);
    panel->add(buildHintCard());

    dic.gui.rebuildWith(layout);
}

tgui::Container::Ptr AppStateHint::buildHintCard()
{
    auto layout = tgui::Group::create();

    auto headingGroup = tgui::Group::create({ "100%", Sizers::getBaseContainerHeight() * 1.5 });
    headingGroup->add(WidgetBuilder::createHeading("hint", HeadingLevel::H2));
    layout->add(headingGroup, "HintHeading");

    auto buttonGroup = tgui::Group::create({ "100%", Sizers::getBaseContainerHeight() });
    buttonGroup->setPosition({ "0%", "parent.height - height" });
    buttonGroup->add(
        WidgetBuilder::createButton(
            dic.strings.getString(StringId::Continue), [&] { onContinue(); }));
    layout->add(buttonGroup, "HintSubmit");

    auto checkboxGroup = tgui::HorizontalLayout::create({ "100%", Sizers::getBaseContainerHeight() });
    checkboxGroup->setPosition({ "0%", "parent.height - height - HintSubmit.height" });
    checkboxGroup->add(
        WidgetBuilder::createTextLabel(
        dic.strings.getString(StringId::ShowHints), "justify"_true));

    auto&& checkboxWrap = tgui::Group::create();
    auto&& checkbox = WidgetBuilder::createCheckbox(
        settings.features.showHints,
        [&](bool value) { settings.features.showHints = value; });
    checkbox->setSize({ Sizers::getBaseContainerHeight(), Sizers::getBaseContainerHeight() });
    checkbox->setPosition({ "parent.width / 2 - width / 2", "0%" });
    checkboxWrap->add(checkbox);
    checkboxGroup->add(checkboxWrap);
    layout->add(checkboxGroup, "HintCheckbox");

    auto textGroup = tgui::Group::create({ "90%", "parent.height - HintHeading.height - HintSubmit.height - HintCheckbox.height" });
    textGroup->setPosition({ "5%", "HintHeading.height" });
    textGroup->add(WidgetBuilder::createTextLabel(message, "justify"_true));
    layout->add(textGroup);    

    return layout;
}

void AppStateHint::onContinue()
{
    app.popState();
}
