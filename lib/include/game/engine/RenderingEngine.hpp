#pragma once

#include "game/Box2d.hpp"
#include "game/Scene.hpp"
#include "game/TiledLevel.hpp"
#include "misc/FpsCounter.hpp"
#include "settings/VideoSettings.hpp"
#include <DGM/dgm.hpp>

class [[nodiscard]] RenderingEngine final
{
public:
    RenderingEngine(
        dgm::Window& window,
        dgm::ResourceManager& resmgr,
        const VideoSettings& settings,
        Scene& scene,
        const TiledLevel& level) noexcept;

public:
    void update(const dgm::Time& time);

    void draw(dgm::Window& window);

    void RenderWorld();

    void RenderHUD();

private:
    dgm::Window& window;
    const VideoSettings& settings;
    Scene& scene;
    BoxDebugRenderer boxDebugRenderer;
    dgm::Camera worldCamera;
    dgm::Camera hudCamera;
    FpsCounter fpsCounter;
    sf::Text text;

    dgm::TileMap tileMap;
    sf::Sprite sprite;
    dgm::AnimationStates ballStates;
    sf::CircleShape spriteOutline;
    sf::Sprite background;
};
