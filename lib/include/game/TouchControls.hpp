#pragma once

#include "input/Input.hpp"
#include <DGM/classes/Objects.hpp>
#include <DGM/classes/Window.hpp>
#include <SFML/Window/Event.hpp>

class [[nodiscard]] TouchControls final
{
public:
    TouchControls(Input& input, const sf::Vector2u& windowSize);

public:
    void processEvent(const sf::Event::TouchBegan& e);

    void processEvent(const sf::Event::TouchEnded& e);

    void draw(dgm::Window& window);

private:
    Input& input;
    dgm::Circle pauseButton;
    dgm::Circle redButton;
    dgm::Circle blueButton;
    std::map<unsigned, InputKind> fingerToAction;
};
