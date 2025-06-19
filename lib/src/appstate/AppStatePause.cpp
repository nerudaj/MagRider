#include "appstate/AppStatePause.hpp"
#include "appstate/AppStateOptions.hpp"
#include "appstate/CommonHandler.hpp"
#include "appstate/Messaging.hpp"
#include "gui/Builders.hpp"

void AppStatePause::input()
{
    CommonHandler::handleInput(app, dic, settings.input);
}

void AppStatePause::update() {}

void AppStatePause::draw()
{
    dic.gui.draw();
    dic.virtualCursor.draw();
}

void AppStatePause::buildLayout()
{
    dic.gui.rebuildWith(
        DefaultLayoutBuilder()
            .withNoBackgroundImage()
            .withTitle(
                dic.strings.getString(StringId::PauseTitle), HeadingLevel::H1)
            .withContent(ButtonListBuilder()
                             .addButton(
                                 dic.strings.getString(StringId::Resume),
                                 [&] { onResume(); })
                             .addButton(
                                 dic.strings.getString(StringId::Options),
                                 [&] { onOptions(); })
                             .addButton(
                                 dic.strings.getString(StringId::BackToMenu),
                                 [&] { onBackToMenu(); })
                             .addButton(
                                 dic.strings.getString(StringId::ExitButton),
                                 [&] { onExit(); })
                             .build())
            .withNoBackButton()
            .withNoSubmitButton()
            .build());
}

void AppStatePause::onResume()
{
    app.popState();
}

void AppStatePause::onOptions()
{
    app.pushState<AppStateOptions>(dic, settings);
}

void AppStatePause::onBackToMenu()
{
    app.popState(Messaging::serialize<PopIfNotMenu>());
}

void AppStatePause::onExit()
{
    app.exit();
}

void AppStatePause::restoreFocusImpl(const std::string&)
{
    buildLayout();
}
