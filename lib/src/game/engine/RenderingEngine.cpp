#include "game/engine/RenderingEngine.hpp"

void RenderingEngine::update(const dgm::Time& time)
{
    fpsCounter.update(time.getDeltaTime());
}

void RenderingEngine::draw(dgm::Window& window)
{
    scene.camera.setPosition(scene.position);
    sprite.setPosition(scene.position);

    window.setViewFromCamera(scene.camera);

    window.draw(scene.tileMap);
    window.draw(sprite);

    text.setString(fpsCounter.getText());
    window.draw(text);
}