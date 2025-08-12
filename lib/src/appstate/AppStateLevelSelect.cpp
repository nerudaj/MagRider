#include "appstate/AppStateLevelSelect.hpp"
#include "appstate/AppStateGameWrapper.hpp"
#include "appstate/CommonHandler.hpp"
#include "appstate/Messaging.hpp"
#include "filesystem/AppStorage.hpp"
#include "filesystem/models/TiledModels.hpp"
#include "game/Constants.hpp"
#include "gui/Builders.hpp"
#include "misc/Utility.hpp"
#include <array>

/// <summary>
/// Remaps levels based on their perceived difficulty
/// </summary>
constexpr const std::array<size_t, 30> DIFFICULTY_REMAPPER = {
    11, 1,  /*4,*/ 16, 2,  3,  12, /*7,*/ /*6,*/ 13,
    10, 15, 8,         9,  5,  17, 14,
    18, 19, 20,        21, 22, 23, 24,
    33, 26, 27,        25, 32, 30, 31,
    29, 28
};

AppStateLevelSelect::AppStateLevelSelect(
    dgm::App& app, DependencyContainer& dic, AppSettings& settings) noexcept
    : dgm::AppState(app)
    , dic(dic)
    , settings(settings)
    , levelIds(dic.resmgr.getLoadedResourceIds<tiled::FiniteMapModel>().value())
    , lastSelectedTab(dic.strings.getString(StringId::Grasslands))
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
    lastSelectedTab = tabName;

    if (tabName == dic.strings.getString(StringId::Grasslands))
    {
        buildContentGrasslands();
    }
    else if (tabName == dic.strings.getString(StringId::Factory))
    {
        buildContentFactory();
    }
    else
    {
        throw std::runtime_error(
            uni::format("Unsupported tab name {}", tabName.toStdString()));
    }
}

void AppStateLevelSelect::buildLayout()
{
    dic.gui.rebuildWith(
        DefaultLayoutBuilder()
            .withBackgroundImage(
                dic.resmgr.get<sf::Texture>("background-forest.png"))
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
                        { onTabClicked(tabName); },
                        WidgetOptions { .id = "LevelSelectTabs" }))
                    .withContent(content)
                    .build())
            .withNoTopLeftButton()
            .withNoTopRightButton()
            .withBottomLeftButton(WidgetBuilder::createButton(
                dic.strings.getString(StringId::Back), [&] { app.popState(); }))
            .withNoBottomRightButton()
            .build());

    auto tabs = dic.gui.get<tgui::Tabs>("LevelSelectTabs");
    tabs->deselect();
    tabs->select(lastSelectedTab);
    //    buildContentGrasslands();
}

void AppStateLevelSelect::buildLevelCards(
    const size_t startIdx,
    const size_t columnCount,
    const size_t rowCount) const
{
    auto&& grid = tgui::Grid::create();
    grid->setSize({ "100%", "100%" });

    auto&& bestTimes =
        std::vector<std::optional<float>>(columnCount * rowCount, std::nullopt);

    for (unsigned y = 0; y < rowCount; ++y)
    {
        for (unsigned x = 0; x < columnCount; ++x)
        {
            const auto levelIdx = y * columnCount + x + startIdx;
            if (levelIdx >= levelIds.size()) break;

            bestTimes[levelIdx - startIdx] =
                Utility::getBestTime(settings.save, levelIdx);

            const bool isUnlocked =
                levelIdx == startIdx
                || bestTimes[levelIdx - startIdx - 1].has_value()
                || (y > 0
                    && bestTimes[levelIdx - startIdx - columnCount]
                           .has_value());
            grid->addWidget(
                buildLevelCard(
                    levelIdx, isUnlocked, bestTimes[levelIdx - startIdx]),
                y,
                x);
        }
    }

    content->removeAllWidgets();
    content->add(grid);
}

void AppStateLevelSelect::buildContentGrasslands() const
{
    dic.gui.get<tgui::Panel>("RootContainer")
        ->getRenderer()
        ->setTextureBackground(
            dic.resmgr.get<tgui::Texture>("background-forest.png"));
    buildLevelCards(0, 5, 3);
}

void AppStateLevelSelect::buildContentFactory() const
{
    dic.gui.get<tgui::Panel>("RootContainer")
        ->getRenderer()
        ->setTextureBackground(
            dic.resmgr.get<tgui::Texture>("background-city.png"));
    buildLevelCards(15, 5, 3);
}

tgui::Container::Ptr AppStateLevelSelect::buildLevelCard(
    size_t levelIdx,
    bool isUnlocked,
    const std::optional<float>& bestTime) const
{
    auto&& card = WidgetBuilder::createPanel(
        { "16%", "30%" }, bestTime ? COLOR_ORANGE : COLOR_DARK_GREY);
    card->getRenderer()->setBorders({ 2.f });
    card->getRenderer()->setBorderColor(
        isUnlocked ? COLOR_YELLOW : COLOR_BLACK);

    auto&& headerPanel = WidgetBuilder::createPanel({ "100%", "40%" });
    auto&& timePanel = WidgetBuilder::createPanel({ "100%", "30%" });
    timePanel->setPosition({ "0%", "40%" });
    auto&& buttonPanel = WidgetBuilder::createPanel({ "100%", "30%" });
    buttonPanel->setPosition({ "0%", "70%" });

    card->add(headerPanel);
    card->add(timePanel);

    if (isUnlocked) card->add(buttonPanel);

    auto&& timeText = std::string("--:--");
    if (bestTime) timeText = Utility::formatTime(bestTime.value());
    if (!isUnlocked) timeText = dic.strings.getString(StringId::Locked);

    auto&& createLabel = [&](const std::string& str)
    {
        auto&& label = WidgetBuilder::createTextLabel(str, "justify"_true);
        return label;
    };

    auto onClick = [&, idx = levelIdx]
    {
        bool useGrass = idx < 15;
        bool useDefaultJoe = (rand() % 3) < 2;
        dic.jukebox.playIngameTrack();
        app.pushState<AppStateGameWrapper>(
            dic,
            settings,
            GameConfig {
                .levelIdx = idx,
                .levelResourceName = levelIds
                    [idx < DIFFICULTY_REMAPPER.size()
                         ? DIFFICULTY_REMAPPER[idx] - 1
                         : idx],
                .tilesetName =
                    useGrass ? "grass_tileset.png" : "metal_tileset.png",
                .joeSkinName = useDefaultJoe ? "base" : "metal",
                .backgroundName =
                    useGrass ? "background-forest.png" : "background-city.png",
            });
    };

    headerPanel->add(createLabel(std::to_string(levelIdx + 1)));
    timePanel->add(createLabel(timeText));
    buttonPanel->add(WidgetBuilder::createButton(
        dic.strings.getString(StringId::PlayButton), onClick));

    if (isUnlocked) card->onClick(onClick);

    return card;
}
