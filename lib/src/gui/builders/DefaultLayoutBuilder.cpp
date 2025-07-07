#include "gui/builders/DefaultLayoutBuilder.hpp"
#include "gui/Sizers.hpp"
#include "gui/TguiHelper.hpp"
#include "gui/builders/WidgetBuilder.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

namespace priv
{
    static tgui::Panel::Ptr containButton(
        tgui::Button::Ptr button,
        const BuilderProperties& props,
        tgui::HorizontalAlignment align,
        tgui::VerticalAlignment valign)
    {
        assert(
            align != tgui::HorizontalAlignment::Center
            && valign != tgui::VerticalAlignment::Center);

        auto&& panel = WidgetBuilder::createPanel(
            { props.cornerButtonDimension, props.cornerButtonDimension });
        panel->setPosition({
            (align == tgui::HorizontalAlignment::Left
                 ? uni::format("{}", props.cornerButtonPadding)
                 : uni::format(
                       "100% - {} - {}",
                       props.cornerButtonPadding,
                       props.cornerButtonDimension))
                .c_str(),
            (valign == tgui::VerticalAlignment::Top
                 ? uni::format("{}", props.cornerButtonPadding)
                 : uni::format(
                       "100% - {} - {}",
                       props.cornerButtonPadding,
                       props.cornerButtonDimension))
                .c_str(),
        });
        panel->add(button);
        return panel;
    }

    FinalizedLayoutBuilder
    LayoutBuilderWithContentAndThreeButtons::withBottomRightButton(
        tgui::Button::Ptr button)
    {
        container->add(containButton(
            button,
            props,
            tgui::HorizontalAlignment::Right,
            tgui::VerticalAlignment::Bottom));
        return FinalizedLayoutBuilder(container, props);
    }

    LayoutBuilderWithContentAndThreeButtons
    LayoutBuilderWithContentAndTwoButtons::withBottomLeftButton(
        tgui::Button::Ptr button)
    {
        container->add(containButton(
            button,
            props,
            tgui::HorizontalAlignment::Left,
            tgui::VerticalAlignment::Bottom));
        return LayoutBuilderWithContentAndThreeButtons(container, props);
    }

    LayoutBuilderWithContentAndTwoButtons
    LayoutBuilderWithContentAndOneButton::withTopRightButton(
        tgui::Button::Ptr button)
    {
        container->add(containButton(
            button,
            props,
            tgui::HorizontalAlignment::Right,
            tgui::VerticalAlignment::Top));
        return LayoutBuilderWithContentAndTwoButtons(container, props);
    }

    LayoutBuilderWithContentAndOneButton
    LayoutBuilderWithContent::withTopLeftButton(tgui::Button::Ptr button)
    {
        container->add(containButton(
            button,
            props,
            tgui::HorizontalAlignment::Left,
            tgui::VerticalAlignment::Top));
        return LayoutBuilderWithContentAndOneButton(container, props);
    }

    LayoutBuilderWithContent LayoutBuilderWithBackgroundAndTitle::withContent(
        tgui::Container::Ptr content)
    {
        auto&& contentPanel = WidgetBuilder::createPanel({
            "70%",
            uni::format(
                "100% - {} - {} - {}",
                props.titleHeight,
                props.cornerButtonDimension,
                props.cornerButtonPadding)
                .c_str(),
        });
        contentPanel->setPosition({ "15%", props.titleHeight });
        contentPanel->add(content);
        container->add(contentPanel);
        return LayoutBuilderWithContent(container, props);
    }

    LayoutBuilderWithBackgroundAndTitle LayoutBuilderWithBackground::withTitle(
        const std::string& title, HeadingLevel level)
    {
        auto&& panel =
            WidgetBuilder::createPanel({ "100%", props.titleHeight });
        panel->add(WidgetBuilder::createHeading(title, level));
        container->add(panel);
        return LayoutBuilderWithBackgroundAndTitle(container, props);
    }
} // namespace priv

priv::LayoutBuilderWithBackground
DefaultLayoutBuilder::withBackgroundImage(const sf::Texture& texture)
{
    auto&& bgr = WidgetBuilder::createPanel();
    bgr->getRenderer()->setTextureBackground(
        TguiHelper::convertTexture(texture));
    return priv::LayoutBuilderWithBackground(bgr, buildProperties());
}

priv::LayoutBuilderWithBackground DefaultLayoutBuilder::withNoBackgroundImage()
{
    return priv::LayoutBuilderWithBackground(
        WidgetBuilder::createPanel(), buildProperties());
}

priv::BuilderProperties DefaultLayoutBuilder::buildProperties()
{
    const auto baseHeight = Sizers::getBaseContainerHeight();

    return priv::BuilderProperties {
        .baseHeight = baseHeight,
        .cornerButtonDimension = baseHeight * 2,
        .cornerButtonPadding = baseHeight / 2,
        .titleHeight = 3 * baseHeight,
    };
}
