#include "appstate/AppStateLevelEnd.hpp"
#include "appstate/CommonHandler.hpp"
#include "appstate/Messaging.hpp"
#include "game/Constants.hpp"
#include "gui/Builders.hpp"
#include "gui/Sizers.hpp"
#include "misc/Utility.hpp"

AppStateLevelEnd::AppStateLevelEnd(
    dgm::App& app,
    DependencyContainer& dic,
    AppSettings& settings,
    size_t levelIdx,
    float levelTime)
    : dgm::AppState(app), dic(dic), settings(settings)
{
    buildLayout(
        Utility::setBestTime(settings.save, levelIdx, levelTime), levelTime);
}

void AppStateLevelEnd::input()
{
    CommonHandler::handleInput(
        app,
        dic,
        settings.input,
        CommonHandlerOptions { .disableGoBack = true });
}

void AppStateLevelEnd::update()
{
    blinkTimeout -= app.time.getElapsed();
    if (blinkTimeout <= sf::Time::Zero)
    {
        auto label = dic.gui.get<tgui::Label>("NewBestLabel");
        auto color = label->getRenderer()->getTextColor();
        label->getRenderer()->setTextColor(
            color == tgui::Color(COLOR_WHITE) ? COLOR_DARK_PURPLE
                                              : COLOR_WHITE);
        blinkTimeout = sf::seconds(0.5f);
    }
}

void AppStateLevelEnd::draw()
{
    dic.gui.draw();
    dic.virtualCursor.draw();
}

void AppStateLevelEnd::buildLayout(bool isNewBest, float levelTime)
{
    dic.gui.rebuildWith(
        DefaultLayoutBuilder()
            .withBackgroundImage(
                dic.resmgr.get<sf::Texture>("menu-background.png"))
            .withTitle(
                dic.strings.getString(StringId::LevelFinished),
                HeadingLevel::H1)
            .withContent(buildContent(isNewBest, levelTime))
            .withNoTopLeftButton()
            .withNoTopRightButton()
            .withBottomLeftButton(WidgetBuilder::createButton(
                dic.strings.getString(StringId::Retry),
                [&] { app.popState(Messaging::serialize<RestartLevel>()); }))
            .withBottomRightButton(WidgetBuilder::createButton(
                dic.strings.getString(StringId::Continue),
                [&]
                {
                    app.popState(
                        Messaging::serialize<PopIfNotLevelSelection>());
                }))
            .build());

    auto content = dic.gui.get<tgui::Container>("DefaultLayoutContentPanel");
    auto label = dic.gui.get<tgui::Label>("TimeLabel");
    content->setWidth(label->getMaximumTextWidth());
}

tgui::Container::Ptr
AppStateLevelEnd::buildContent(bool isNewBest, float levelTime)
{
    auto baseSize = Sizers::getBaseFontSize();
    auto content =
        WidgetBuilder::createPanel({ "40%", 13 * baseSize }, COLOR_ORANGE);
    content->setPosition(
        { "parent.width / 2 - width / 2", "parent.height / 2 - height / 2" });
    content->getRenderer()->setBorders({ 2.f });
    content->getRenderer()->setBorderColor(COLOR_YELLOW);

    auto heading = WidgetBuilder::createPanel({ "100%", baseSize * 3.f });
    heading->setPosition({ "0%", baseSize * 1.f });
    auto headingLabel = WidgetBuilder::createTextLabel(
        dic.strings.getString(StringId::YourTime), true);
    headingLabel->setTextSize(baseSize * 2);
    heading->add(headingLabel);

    auto time = WidgetBuilder::createPanel({ "100%", baseSize * 5.f });
    time->setPosition({ "0%", baseSize * 4.f });
    auto timeLabel =
        WidgetBuilder::createTextLabel(Utility::formatTime(levelTime), true);
    timeLabel->setTextSize(baseSize * 3);
    time->add(timeLabel, "TimeLabel");

    auto newBest = WidgetBuilder::createPanel({ "100%", baseSize * 3.f });
    newBest->setPosition({ "0%", baseSize * 9.f });
    auto newBestLabel = WidgetBuilder::createTextLabel(
        isNewBest ? dic.strings.getString(StringId::NewBest) : "", true);
    newBestLabel->setTextSize(baseSize * 2);
    newBest->add(newBestLabel, "NewBestLabel");

    content->add(heading);
    content->add(time);
    content->add(newBest);

    return content;
}
