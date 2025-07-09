#include "appstate/AppStatePause.hpp"
#include "appstate/AppStateOptions.hpp"
#include "appstate/CommonHandler.hpp"
#include "appstate/Messaging.hpp"
#include "gui/Builders.hpp"
#include "gui/Icon.hpp"
#include "gui/Sizers.hpp"

sf::Image createMask(const sf::Vector2u& size)
{
    auto&& image = sf::Image(size, sf::Color(255, 255, 255, 96));
    return image;
}

AppStatePause::AppStatePause(
    dgm::App& app, DependencyContainer& dic, AppSettings& settings) noexcept
    : dgm::AppState(
          app,
          dgm::AppStateConfig {
              .shouldDrawUnderlyingState = true,
          })
    , dic(dic)
    , settings(settings)
{
    std::ignore = background.loadFromImage(createMask(app.window.getSize()));
    buildLayout();
}

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
    auto&& texture = dic.resmgr.get<sf::Texture>("pixel-ui-icons.png");
    auto&& clip = dic.resmgr.get<dgm::Clip>("pixel-ui-icons.png.clip");

#ifdef ANDROID
    dic.gui.rebuildWith(
        DefaultLayoutBuilder()
            .withBackgroundImage(background)
            .withTitle(
                dic.strings.getString(StringId::PauseTitle), HeadingLevel::H2)
            .withContent(
                ButtonListBuilder()
                    .addButton(
                        dic.strings.getString(StringId::BackToLevelSelect),
                        [&] { onBackToMenu(); })
                    .addButton(
                        dic.strings.getString(StringId::ExitButton),
                        [&] { onExit(); })
                    .build())
            .withTopLeftButton(WidgetBuilder::createTexturedButton(
                texture, clip.getFrame(Icon::PlayFill), [&] { onResume(); }))
            .withTopRightButton(WidgetBuilder::createTexturedButton(
                texture,
                clip.getFrame(Icon::SettingsFill),
                [&] { onOptions(); }))
            .withBottomLeftButton(WidgetBuilder::createTexturedButton(
                texture, clip.getFrame(Icon::Restart), [&] { onRestart(); }))
            .withNoBottomRightButton()
            .build());
#else
    dic.gui.rebuildWith(
        DefaultLayoutBuilder()
            .withBackgroundImage(background)
            .withTitle(
                dic.strings.getString(StringId::PauseTitle), HeadingLevel::H1)
            .withContent(
                ButtonListBuilder()

                    .addButton(
                        dic.strings.getString(StringId::Resume),
                        [&] { onResume(); })
                    .addButton(
                        dic.strings.getString(StringId::Restart),
                        [&] { onRestart(); })
                    .addButton(
                        dic.strings.getString(StringId::BackToLevelSelect),
                        [&] { onBackToMenu(); })
                    .addButton(
                        dic.strings.getString(StringId::Options),
                        [&] { onOptions(); })
                    .addButton(
                        dic.strings.getString(StringId::ExitButton),
                        [&] { onExit(); })
                    .build())
            .withNoCornerButtons()
            .build());
#endif
}

void AppStatePause::onResume()
{
    app.popState();
}

void AppStatePause::onRestart()
{
    app.popState(Messaging::serialize<RestartLevel>());
}

void AppStatePause::onOptions()
{
    app.pushState<AppStateOptions>(dic, settings);
}

void AppStatePause::onBackToMenu()
{
    app.popState(Messaging::serialize<PopIfNotLevelSelection>());
}

void AppStatePause::onExit()
{
    app.exit();
}

void AppStatePause::restoreFocusImpl(const std::string&)
{
    buildLayout();
}
