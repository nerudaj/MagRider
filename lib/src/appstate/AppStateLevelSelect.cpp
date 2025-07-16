#include "appstate/AppStateLevelSelect.hpp"
#include "appstate/AppStateGameWrapper.hpp"
#include "appstate/CommonHandler.hpp"
#include "appstate/Messaging.hpp"
#include "filesystem/models/TiledModels.hpp"
#include "game/Constants.hpp"
#include "gui/Builders.hpp"
#include "misc/Utility.hpp"

AppStateLevelSelect::AppStateLevelSelect(
    dgm::App& app, DependencyContainer& dic, AppSettings& settings) noexcept
    : dgm::AppState(app)
    , dic(dic)
    , settings(settings)
    , levelIds(dic.resmgr.getLoadedResourceIds<tiled::FiniteMapModel>().value())
{
    std::ranges::sort(levelIds);
    content = WidgetBuilder::createPanel();
    buildLayout();
}

void AppStateLevelSelect::input()
{
    CommonHandler::handleInput(app, dic, settings.input);
}

void AppStateLevelSelect::update() {}

void AppStateLevelSelect::draw()
{
    dic.gui.draw();
    dic.virtualCursor.draw();
}

void AppStateLevelSelect::restoreFocusImpl(const std::string& message)
{
    dic.jukebox.playTitleTrack();
    auto msg = Messaging::deserialize(message);
    if (msg)
        std::visit(
            overloads {
                [&](PopIfNotLevelSelection) { buildLayout(); },
                [&](auto) { app.popState(message); },
            },
            *msg);
    else
        app.popState(message);
}

void AppStateLevelSelect::onTabClicked(const tgui::String& tabName)
{
    if (tabName == dic.strings.getString(StringId::Grasslands))
        buildContentGrasslands();
    else if (tabName == dic.strings.getString(StringId::Factory))
        buildContentFactory();
}

void AppStateLevelSelect::buildLayout()
{
    dic.gui.rebuildWith(
        DefaultLayoutBuilder()
            .withBackgroundImage(
                dic.resmgr.get<sf::Texture>("menu-background.png"))
            .withTitle(
                dic.strings.getString(StringId::SelectLevel),
#ifdef ANDROID
                HeadingLevel::H2)
#else
                HeadingLevel::H1)
#endif
            .withContent(
                NavbarLayoutBuilder()
                    .withNavbarWidget(WidgetBuilder::createTabbedContent(
                        { dic.strings.getString(StringId::Grasslands),
                          dic.strings.getString(StringId::Factory) },
                        [&](const tgui::String& tabName)
                        { onTabClicked(tabName); }))
                    .withContent(content)
                    .build())
            .withNoTopLeftButton()
            .withNoTopRightButton()
            .withBottomLeftButton(WidgetBuilder::createButton(
                dic.strings.getString(StringId::Back), [&] { app.popState(); }))
            .withNoBottomRightButton()
            .build());

    buildContentGrasslands();
}

void AppStateLevelSelect::buildContentGrasslands() const
{
    auto&& grid = tgui::Grid::create();
    grid->setSize({ "100%", "100%" });

    const unsigned ROW_COUNT = 3;
    const unsigned COL_COUNT = 6;
    for (unsigned y = 0; y < ROW_COUNT; ++y)
    {
        for (unsigned x = 0; x < COL_COUNT; ++x)
        {
            const unsigned levelIdx = y * COL_COUNT + x;
            if (levelIdx >= 18) break;
            grid->addWidget(buildLevelCard(levelIdx), y, x);
        }
    }

    content->removeAllWidgets();
    content->add(grid);
}

void AppStateLevelSelect::buildContentFactory() const
{
    auto&& grid = tgui::Grid::create();
    grid->setSize({ "100%", "100%" });

    const unsigned ROW_COUNT = 3;
    const unsigned COL_COUNT = 6;
    for (unsigned y = 0; y < ROW_COUNT; ++y)
    {
        for (unsigned x = 0; x < COL_COUNT; ++x)
        {
            const unsigned levelIdx = y * COL_COUNT + x + 18;
            if (levelIdx >= levelIds.size()) break;
            grid->addWidget(buildLevelCard(levelIdx), y, x);
        }
    }

    content->removeAllWidgets();
    content->add(grid);
}

tgui::Container::Ptr AppStateLevelSelect::buildLevelCard(size_t levelIdx) const
{
    const auto&& bestTime = Utility::getBestTime(settings.save, levelIdx);

    auto&& card = WidgetBuilder::createPanel(
        { "16%", "30%" }, bestTime ? COLOR_ORANGE : COLOR_DARK_GREY);
    card->getRenderer()->setBorders({ 2.f });
    card->getRenderer()->setBorderColor(COLOR_YELLOW);
    auto&& headerPanel = WidgetBuilder::createPanel({ "100%", "40%" });
    auto&& timePanel = WidgetBuilder::createPanel({ "100%", "30%" });
    timePanel->setPosition({ "0%", "40%" });
    auto&& buttonPanel = WidgetBuilder::createPanel({ "100%", "30%" });
    buttonPanel->setPosition({ "0%", "70%" });

    card->add(headerPanel);
    card->add(timePanel);
    card->add(buttonPanel);

    auto&& timeText = std::string("--:--");
    if (bestTime) timeText = Utility::formatTime(bestTime.value());

    auto&& createLabel = [&](const std::string& str)
    {
        auto&& label = WidgetBuilder::createTextLabel(str, "justify"_true);
        label->getRenderer()->setTextColor(COLOR_DARK_GREY);
        return label;
    };

    headerPanel->add(createLabel(std::to_string(levelIdx + 1)));
    timePanel->add(createLabel(timeText));
    buttonPanel->add(WidgetBuilder::createButton(
        dic.strings.getString(StringId::PlayButton),
        [&, idx = levelIdx]
        {
            dic.jukebox.playIngameTrack();
            app.pushState<AppStateGameWrapper>(
                dic, settings, GameConfig { idx, levelIds[idx] });
        }));

    return card;
}
