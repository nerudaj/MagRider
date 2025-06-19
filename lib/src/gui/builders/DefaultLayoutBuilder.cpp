#include "gui/Sizers.hpp"
#include "gui/TguiHelper.hpp"
#include "gui/builders/DefaultLayoutBuilder.hpp"
#include "gui/builders/WidgetBuilder.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

auto getTitleHeight()
{
    return Sizers::getBaseContainerHeight() * 4.f;
}

namespace priv
{
    FinalizedLayoutBuilder
    LayoutBuilderWithContentAndBackButton::withSubmitButton(
        tgui::Button::Ptr button)
    {
        auto&& panel = WidgetBuilder::createPanel(
            { "15%", Sizers::getBaseContainerHeight() });
        panel->setPosition(
            { "84%",
              ("99% - " + std::to_string(Sizers::getBaseContainerHeight()))
                  .c_str() });
        panel->add(button);
        container->add(panel);
        return FinalizedLayoutBuilder(container);
    }

    LayoutBuilderWithContentAndBackButton
    LayoutBuilderWithContent::withBackButton(tgui::Button::Ptr button)
    {
        auto&& panel = WidgetBuilder::createPanel(
            { "15%", Sizers::getBaseContainerHeight() });
        panel->setPosition(
            { "1%",
              ("99% - " + std::to_string(Sizers::getBaseContainerHeight()))
                  .c_str() });
        panel->add(button);
        container->add(panel);
        return LayoutBuilderWithContentAndBackButton(container);
    }

    LayoutBuilderWithContent LayoutBuilderWithBackgroundAndTitle::withContent(
        tgui::Container::Ptr content)
    {
        auto&& contentPanel = WidgetBuilder::createPanel(
            { "70%",
              ("98% - " + std::to_string(getTitleHeight()) + " - "
               + std::to_string(Sizers::getBaseContainerHeight()))
                  .c_str() });
        contentPanel->setPosition({ "15%", getTitleHeight() });
        contentPanel->add(content);
        container->add(contentPanel);
        return LayoutBuilderWithContent(container);
    }

    LayoutBuilderWithBackgroundAndTitle LayoutBuilderWithBackground::withTitle(
        const std::string& title, HeadingLevel level)
    {
        auto&& panel = WidgetBuilder::createPanel({ "100%", getTitleHeight() });
        panel->add(WidgetBuilder::createHeading(title, level));
        container->add(panel);
        return LayoutBuilderWithBackgroundAndTitle(container);
    }
} // namespace priv

priv::LayoutBuilderWithBackground
DefaultLayoutBuilder::withBackgroundImage(const sf::Texture& texture)
{
    auto&& bgr = WidgetBuilder::createPanel();
    bgr->getRenderer()->setTextureBackground(
        TguiHelper::convertTexture(texture));
    return priv::LayoutBuilderWithBackground(bgr);
}

priv::LayoutBuilderWithBackground DefaultLayoutBuilder::withNoBackgroundImage()
{
    return priv::LayoutBuilderWithBackground(WidgetBuilder::createPanel());
}
