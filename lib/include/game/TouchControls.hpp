#pragma once

#include "input/Input.hpp"
#include "settings/InputSettings.hpp"
#include <DGM/classes/Objects.hpp>
#include <DGM/classes/ResourceManager.hpp>
#include <DGM/classes/Window.hpp>
#include <SFML/Window/Event.hpp>

class [[nodiscard]] TouchControls final
{
public:
    TouchControls(
        const dgm::ResourceManager& resmgr,
        Input& input,
        const InputSettings& settings,
        const sf::Vector2u& windowSize);

public:
    void processEvent(const sf::Event::TouchBegan& e);

    void processEvent(const sf::Event::TouchEnded& e);

    void draw(dgm::Window& window);

    void regenerateButtons(
        const sf::Vector2u& windowSize, const InputSettings& settings);

private:
    Input& input;
    sf::Sprite pauseButtonSprite;
    dgm::Circle pauseButton;
    dgm::Circle redButton;
    dgm::Circle blueButton;
    std::map<unsigned, InputKind> fingerToAction;
};
