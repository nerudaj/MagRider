#include "appstate/AppStateOptions.hpp"
#include "appstate/CommonHandler.hpp"
#include "game/Constants.hpp"
#include "gui/Builders.hpp"
#include "gui/Sizers.hpp"
#include "misc/Compatibility.hpp"
#include "strings/InputKindToStringMapper.hpp"
#include "types/Overloads.hpp"
#include <ranges>

constexpr const char* TABS_ID = "Options_Tabs";

std::string resolutionToString(const sf::Vector2u& vec)
{
    return uni::format("{}x{}", vec.x, vec.y);
}

static std::vector<std::string> getResolutionStrings()
{
    return sf::VideoMode::getFullscreenModes()
           | std::views::transform([](const sf::VideoMode& mode)
                                   { return mode.size; })
           | std::views::transform(resolutionToString)
           | uniranges::to<std::vector>();
}

static std::string intValueFormatter(float val)
{
    return std::to_string(static_cast<int>(val));
}

AppStateOptions::AppStateOptions(
    dgm::App& app, DependencyContainer& dic, AppSettings& settings) noexcept
    : dgm::AppState(app), dic(dic), settings(settings)
{
    refresh();
}

void AppStateOptions::input()
{
    if (inputDetector.isDetectionInProgress())
    {
        inputDetector.update(app.time);
        CommonHandler::swallowAllEvents(app);
        return;
    }

    CommonHandler::handleInput(app, dic, settings.input);

    auto tabs = dic.gui.get<tgui::Tabs>(TABS_ID);
    if (dic.input.isMenuCycleLeftPressed())
    {
        tabs->select(
            (tabs->getSelectedIndex() + tabs->getTabsCount() - 1)
            % tabs->getTabsCount());
    }
    else if (dic.input.isMenuCycleRightPressed())
    {
        tabs->select((tabs->getSelectedIndex() + 1) % tabs->getTabsCount());
    }
}

void AppStateOptions::update() {}

void AppStateOptions::draw()
{
    dic.gui.draw();
    dic.virtualCursor.draw();
}

void AppStateOptions::buildLayout()
{
    auto tabLabels = std::vector<std::string> {
#ifndef ANDROID
        dic.strings.getString(StringId::VideoOptionsTab),
#endif
        dic.strings.getString(StringId::AudioOptionsTab),
    };

    if (settings.features.showInputSettings)
    {
        const auto extraLabels = std::vector<std::string> {
            dic.strings.getString(StringId::InputOptionsTab),
            dic.strings.getString(StringId::BindingsOptionsTab)
        };
        tabLabels.insert(
            tabLabels.end(), extraLabels.begin(), extraLabels.end());
    }

    dic.gui.rebuildWith(
        DefaultLayoutBuilder()
            .withBackgroundImage(
                dic.resmgr.get<sf::Texture>("background-trees.png"))
            .withTitle(
                dic.strings.getString(StringId::Options),
#ifdef ANDROID
                HeadingLevel::H2)
#else
                HeadingLevel::H1)
#endif
            .withContent(
                NavbarLayoutBuilder()
                    .withNavbarWidget(WidgetBuilder::createTabbedContent(
                        tabLabels,
                        [&](const tgui::String& tabName)
                        { onTabClicked(tabName); },
                        WidgetOptions {
                            .id = TABS_ID,
                        }))
                    .withContent(content)
                    .build())
            .withNoTopLeftButton()
            .withNoTopRightButton()
            .withBottomLeftButton(WidgetBuilder::createButton(
                dic.strings.getString(StringId::Back), [&] { onBack(); }))
            .withNoBottomRightButton()
            .build());

#ifdef ANDROID
    buildAudioOptionsLayout();
#else
    buildVideoOptionsLayout();
#endif
}

void AppStateOptions::refresh()
{
    // must be recreated, otherwise it disappears for some reason
    content = WidgetBuilder::createScrollablePanel(
        { "100%", "100%" }, tgui::Color(95, 87, 79, 192));

    buildLayout();
}

void AppStateOptions::buildVideoOptionsLayout()
{
    content->removeAllWidgets();
    content->add(
        FormBuilder()
#ifndef ANDROID
            .addOption(
                dic.strings.getString(StringId::EnableFullscreen),
                WidgetBuilder::createCheckbox(
                    settings.video.fullscreen,
                    [&](bool val)
                    {
                        settings.video.fullscreen = val;
                        app.window.toggleFullscreen();
                    }))
            .addOption(
                dic.strings.getString(StringId::SetResolution),
                WidgetBuilder::createDropdown(
                    getResolutionStrings(),
                    resolutionToString(settings.video.resolution),
                    [&](size_t idx)
                    {
                        onResolutionSelected(
                            sf::VideoMode::getFullscreenModes()[idx].size);
                    }))
#endif
            .addOption(
                dic.strings.getString(StringId::SetUiScale),
                WidgetBuilder::createSlider(
                    settings.video.uiScale,
                    [&](float val)
                    {
                        settings.video.uiScale = val;
                        Sizers::setUiScale(val);
                        // TODO: should refresh view, but the user should not be
                        // clicking anything
                    },
                    dic.gui,
                    SliderProperties { .low = 1.f, .high = 2.f, .step = 0.1f }))
#ifdef _DEBUG
            .addOption(
                dic.strings.getString(StringId::SetTheme),
                WidgetBuilder::createDropdown(
                    dic.resmgr.getLoadedResourceIds<tgui::Theme::Ptr>().value(),
                    "",
                    [&](size_t idx)
                    {
                        dic.gui.setTheme(dic.resmgr.get<tgui::Theme::Ptr>(
                            dic.resmgr.getLoadedResourceIds<tgui::Theme::Ptr>()
                                .value()[idx]));
                        refresh();
                    }))
            .addOption(
                dic.strings.getString(StringId::RenderColliders),
                WidgetBuilder::createCheckbox(
                    settings.video.renderColliders,
                    [&](bool value)
                    { settings.video.renderColliders = value; }))
            .addOption(
                dic.strings.getString(StringId::ShowFPS),
                WidgetBuilder::createCheckbox(
                    settings.video.showFps,
                    [&](bool value) { settings.video.showFps = value; }))
#endif
            .build());
}

void AppStateOptions::buildAudioOptionsLayout()
{
    content->removeAllWidgets();
    content->add(
        FormBuilder()
            .addOption(
                dic.strings.getString(StringId::MusicVolume),
                WidgetBuilder::createSlider(
                    settings.audio.musicVolume,
                    [&](float val)
                    {
                        settings.audio.musicVolume = val;
                        dic.jukebox.setVolume(settings.audio.musicVolume);
                    },
                    dic.gui,
                    SliderProperties { .valueFormatter = intValueFormatter,
                                       .low = 0.f,
                                       .high = 100.f,
                                       .step = 1.f }))
            .addOption(
                dic.strings.getString(StringId::SoundVolume),
                WidgetBuilder::createSlider(
                    settings.audio.soundVolume,
                    [&](float val) { settings.audio.soundVolume = val; },
                    dic.gui,
                    SliderProperties { .valueFormatter = intValueFormatter,
                                       .low = 0.f,
                                       .high = 100.f,
                                       .step = 1.f }))
            .build());
}

void AppStateOptions::buildInputOptionsLayout()
{
    content->removeAllWidgets();
    content->add(
        FormBuilder()
            .addOption(
                dic.strings.getString(StringId::GamepadDeadzone),
                WidgetBuilder::createSlider(
                    settings.input.gamepadDeadzone,
                    [&](float val) { settings.input.gamepadDeadzone = val; },
                    dic.gui,
                    SliderProperties { .valueFormatter = intValueFormatter,
                                       .low = 0.f,
                                       .high = 100.f,
                                       .step = 1.f }))
            .addOption(
                dic.strings.getString(StringId::CursorSpeed),
                WidgetBuilder::createSlider(
                    settings.input.cursorSpeed,
                    [&](float val) { settings.input.cursorSpeed = val; },
                    dic.gui,
                    SliderProperties { .valueFormatter = intValueFormatter,
                                       .low = 100.f,
                                       .high = 1000.f,
                                       .step = 10.f }))
            .build());
}

template<class V, class T>
bool doesVariantContain(const V& v, T t)
{
    if (std::holds_alternative<T>(v)) return std::get<T>(v) == t;
    return false;
}

void AppStateOptions::buildBindingsOptionsLayout()
{
    auto&& inputKindMapper = InputKindToStringMapper(dic.strings);
    auto&& hwInputMapper = HwInputToStringMapper();

    auto tableBuilder = TableBuilder().withHeading({
        dic.strings.getString(StringId::BindingHeadingAction),
        dic.strings.getString(StringId::BindingHeadingKMB),
        dic.strings.getString(StringId::BindingsHeadingGamepad),
    });

    auto buttonOrNothing = [](tgui::Button::Ptr ptr,
                              bool useNothing) -> tgui::Widget::Ptr
    {
        if (useNothing) WidgetBuilder::createTextLabel("");
        return ptr;
    };

    auto addSectionToTable = [&](std::map<InputKind, Binding>& bindings)
    {
        for (auto&& [action, binding] : bindings)
        {
            auto&& [kmbBinding, gamepadBinding] = binding;

            const bool isEscapeKey =
                doesVariantContain(kmbBinding, sf::Keyboard::Key::Escape);
            const bool noKmb =
                doesVariantContain(kmbBinding, std::monostate {});
            const bool noGmp =
                doesVariantContain(gamepadBinding, std::monostate {});

            auto label = WidgetBuilder::createTextLabel(
                inputKindMapper.inputKindToString(action));
            label->getRenderer()->setPadding({ "5%", "0%", "0%", "0%" });

            tableBuilder.addRow({
                label,
                buttonOrNothing(
                    WidgetBuilder::createRowButton(
                        std::visit(hwInputMapper, kmbBinding),
                        [&]()
                        {
                            markButtonRebinding<KmbBinding>(action);
                            inputDetector.startCheckingInputs(
                                [&](KmbBinding b)
                                { onInputDetected(action, b, bindings); },
                                [&]()
                                {
                                    onInputDetected(
                                        action, kmbBinding, bindings);
                                });
                        },
                        WidgetOptions { .id =
                                            getBindButtonId<KmbBinding>(action),
                                        .enabled = !isEscapeKey }),
                    noKmb),
                buttonOrNothing(
                    WidgetBuilder::createRowButton(
                        std::visit(hwInputMapper, gamepadBinding),
                        [&]()
                        {
                            markButtonRebinding<GamepadBinding>(action);
                            inputDetector.startCheckingInputs(
                                [&](GamepadBinding b)
                                { onInputDetected(action, b, bindings); },
                                [&]()
                                {
                                    onInputDetected(
                                        action, gamepadBinding, bindings);
                                });
                        },
                        WidgetOptions {
                            .id = getBindButtonId<GamepadBinding>(action),
                        }),
                    noGmp),
            });
        }
    };

    addSectionToTable(settings.bindings.ingameBindings);
    tableBuilder.addSeparator();
    addSectionToTable(settings.bindings.menuBindings);

    content->removeAllWidgets();
    content->add(tableBuilder.build());
}

void AppStateOptions::onTabClicked(const tgui::String& tabName)
{
    if (tabName == dic.strings.getString(StringId::VideoOptionsTab))
        buildVideoOptionsLayout();
    else if (tabName == dic.strings.getString(StringId::AudioOptionsTab))
        buildAudioOptionsLayout();
    else if (tabName == dic.strings.getString(StringId::InputOptionsTab))
        buildInputOptionsLayout();
    else if (tabName == dic.strings.getString(StringId::BindingsOptionsTab))
        buildBindingsOptionsLayout();
}

void AppStateOptions::onBack()
{
    app.popState();
}

void AppStateOptions::onResolutionSelected(const sf::Vector2u& resolution)
{
    settings.video.resolution = resolution;
    app.window.changeResolution(resolution);
    dic.gui.setWindow(app.window.getSfmlWindowContext());

    refresh();

    // TODO: Open "Are you sure dialog?"
}
