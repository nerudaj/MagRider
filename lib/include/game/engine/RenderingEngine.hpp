#pragma once

#include "game/Scene.hpp"
#include "misc/FpsCounter.hpp"
#include <DGM/dgm.hpp>

class [[nodiscard]] RenderingEngine final
{
public:
    RenderingEngine(dgm::ResourceManager& resmgr, Scene& scene) noexcept
        : scene(scene)
        , sprite(resmgr.get<sf::Texture>("mrman.png"))
        , text(resmgr.get<sf::Font>("ChunkFive-Regular.ttf"))
    {
        sprite.setOrigin(
            sf::Vector2f(scene.dummy.animation.getCurrentFrame().size) / 2.f);
    }

    RenderingEngine(RenderingEngine&&) = delete;
    RenderingEngine(const RenderingEngine&) = delete;

public:
    void update(const dgm::Time& time);

    void draw(dgm::Window& window);

private:
    Scene& scene;
    sf::Sprite sprite;
    FpsCounter fpsCounter;
    sf::Text text;
};
