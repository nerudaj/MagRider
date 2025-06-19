#include "game/engine/RenderingEngine.hpp"

void RenderingEngine::update(const dgm::Time& time)
{
    fpsCounter.update(time.getDeltaTime());
}

void RenderingEngine::draw(dgm::Window& window)
{
    text.setString(fpsCounter.getText());
    window.draw(text);
}