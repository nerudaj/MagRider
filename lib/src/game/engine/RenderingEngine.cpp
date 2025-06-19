#include "game/engine/RenderingEngine.hpp"

void RenderingEngine::update(const dgm::Time& time)
{
    fpsCounter.update(time.getDeltaTime());
}

void RenderingEngine::draw(dgm::Window& window)
{
    sprite.setTextureRect(scene.dummy.animation.getCurrentFrame());
    sprite.setPosition(scene.dummy.body.getCenter());
    sprite.setScale({ scene.dummy.facingLeft ? -1.f : 1.f, 1.f });

    scene.dummy.body.debugRender(window); // rendering hitbox
    window.draw(sprite);

    text.setString(fpsCounter.getText());
    window.draw(text);
}