#pragma once

#include "game/Scene.hpp"
#include "misc/FpsCounter.hpp"
#include <DGM/dgm.hpp>

class [[nodiscard]] RenderingEngine final
{
public:
    RenderingEngine(dgm::ResourceManager& resmgr, Scene& scene) noexcept
        : scene(scene)
        , text(resmgr.get<sf::Font>("ChunkFive-Regular.ttf"))
        , sprite(resmgr.get<sf::Texture>("joe.png"))
    {
        sprite.setOrigin(sf::Vector2f { sprite.getTexture().getSize() / 2u });
    }

    RenderingEngine(RenderingEngine&&) = delete;
    RenderingEngine(const RenderingEngine&) = delete;

public:
    void update(const dgm::Time& time);

    void draw(dgm::Window& window);

private:
    Scene& scene;
    FpsCounter fpsCounter;
    sf::Text text;

    sf::Sprite sprite;
};
