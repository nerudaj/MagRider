#include "appstate/AppStateLevelSelect.hpp"
#include "appstate/AppStateGameWrapper.hpp"
#include "appstate/CommonHandler.hpp"
#include "appstate/Messaging.hpp"
#include "filesystem/AppStorage.hpp"
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
    AppStorage::saveFile(SETTINGS_FILE_NAME, settings);
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

void AppStateLevelSelect::buildLevelCards(
    const size_t startIdx,
    const size_t columnCount,
    const size_t rowCount) const
{
    auto&& grid = tgui::Grid::create();
    grid->setSize({ "100%", "100%" });

    for (unsigned y = 0; y < rowCount; ++y)
    {
        for (unsigned x = 0; x < columnCount; ++x)
        {
            const auto levelIdx = y * columnCount + x + startIdx;
            if (levelIdx >= levelIds.size()) break;
            grid->addWidget(buildLevelCard(levelIdx), y, x);
        }
    }

    content->removeAllWidgets();
    content->add(grid);
}

void AppStateLevelSelect::buildContentGrasslands() const
{
    buildLevelCards(0, 6, 3);
}

void AppStateLevelSelect::buildContentFactory() const
{
    buildLevelCards(18, 6, 3);
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
                dic,
                settings,
                GameConfig {
                    .levelIdx = idx,
                    .levelResourceName = levelIds[idx],
                    .tilesetName =
                        idx < 18 ? "grass_tileset.png" : "metal_tileset.png",
                    .joeSkinName = idx < 18 ? "base" : "metal",
                });
        }));

    return card;
}
