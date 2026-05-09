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

    auto headingGroup = tgui::Group::create({ "100%", "20%" });
    headingGroup->add(WidgetBuilder::createHeading("hint", HeadingLevel::H2));
    layout->add(headingGroup);

    auto textGroup = tgui::Group::create({ "90%", "60%" });
    textGroup->setPosition({ "5%", "20%" });
    textGroup->add(WidgetBuilder::createTextLabel(message));
    layout->add(textGroup);

    auto checkboxGroup = tgui::Group::create({ "100%", "10%" });
    checkboxGroup->setPosition({ "0%", "80%" });
    checkboxGroup->add(
        FormBuilder()
            .addOption(
                dic.strings.getString(StringId::ShowHints),
                WidgetBuilder::createCheckbox(
                    settings.features.showHints,
                    [&](bool value) { settings.features.showHints = value; }),
                OptionConfig {
                    .justify = true,
                })
            .build());
    layout->add(checkboxGroup);

    auto buttonGroup = tgui::Group::create({ "100%", "10%" });
    buttonGroup->setPosition({ "0%", "90%" });
    buttonGroup->add(
        WidgetBuilder::createButton(
            dic.strings.getString(StringId::Continue), [&] { onContinue(); }));
    layout->add(buttonGroup);

    return layout;
}

void AppStateHint::onContinue()
{
    app.popState();
}
