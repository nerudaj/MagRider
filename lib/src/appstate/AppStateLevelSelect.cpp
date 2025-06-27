#include "appstate/AppStateLevelSelect.hpp"
#include "appstate/AppStateGameWrapper.hpp"
#include "appstate/CommonHandler.hpp"
#include "appstate/Messaging.hpp"
#include "filesystem/models/TiledModels.hpp"
#include "gui/Builders.hpp"
#include "misc/Utility.hpp"

AppStateLevelSelect::AppStateLevelSelect(
    dgm::App& app, DependencyContainer& dic, AppSettings& settings) noexcept
    : dgm::AppState(app)
    , dic(dic)
    , settings(settings)
    , levelIds(dic.resmgr.getLoadedResourceIds<tiled::FiniteMapModel>().value())
{
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

void AppStateLevelSelect::buildLayout()
{
    dic.gui.rebuildWith(
        DefaultLayoutBuilder()
            .withNoBackgroundImage()
            .withTitle(
                dic.strings.getString(StringId::SelectLevel), HeadingLevel::H1)
            .withContent(buildContent())
            .withBackButton(WidgetBuilder::createButton(
                dic.strings.getString(StringId::Back), [&] { app.popState(); }))
            .withNoSubmitButton()
            .build());
}

tgui::Container::Ptr AppStateLevelSelect::buildContent() const
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
            if (levelIdx >= levelIds.size()) break;
            grid->addWidget(buildLevelCard(levelIdx), y, x);
        }
    }

    return grid;
}

tgui::Container::Ptr AppStateLevelSelect::buildLevelCard(size_t levelIdx) const
{
    auto&& card =
        WidgetBuilder::createPanel({ "16%", "30%" }, sf::Color::White);
    auto&& headerPanel = WidgetBuilder::createPanel({ "100%", "40%" });
    auto&& timePanel = WidgetBuilder::createPanel({ "100%", "30%" });
    timePanel->setPosition({ "0%", "40%" });
    auto&& buttonPanel = WidgetBuilder::createPanel({ "100%", "30%" });
    buttonPanel->setPosition({ "0%", "70%" });

    card->add(headerPanel);
    card->add(timePanel);
    card->add(buttonPanel);

    auto&& timeText = std::string("--:--");
    if (auto time = Utility::getBestTime(settings.save, levelIdx))
        timeText = Utility::formatTime(time.value());

    headerPanel->add(WidgetBuilder::createHeading(std::to_string(levelIdx)));
    timePanel->add(WidgetBuilder::createHeading(timeText, HeadingLevel::H2));
    buttonPanel->add(WidgetBuilder::createButton(
        dic.strings.getString(StringId::PlayButton),
        [&, idx = levelIdx]
        {
            app.pushState<AppStateGameWrapper>(
                dic, settings, GameConfig { idx, levelIds[idx] });
        }));

    return card;
}
