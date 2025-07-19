#pragma once

#include "game/Box2d.hpp"
#include "game/GameConfig.hpp"
#include "game/Scene.hpp"
#include "game/TiledLevel.hpp"
#include "misc/FpsCounter.hpp"
#include "settings/VideoSettings.hpp"
#include "strings/StringProvider.hpp"
#include <DGM/dgm.hpp>

class [[nodiscard]] SimpleAnimation final
{
public:
    SimpleAnimation(size_t animLength, size_t fps) noexcept
        : length(animLength), timePerFrame(sf::seconds(1.f / fps))
    {
    }

public:
    void update(const dgm::Time& time)
    {
        elapsed += time.getElapsed();
        while (elapsed > timePerFrame)
        {
            elapsed -= timePerFrame;
            frame = (frame + 1) % length;
        }
    }

    CONSTEXPR_NODISCARD size_t getFrame() const noexcept
    {
        return frame;
    }

private:
    size_t frame = 0;
    size_t length = 1;
    sf::Time timePerFrame;
    sf::Time elapsed;
};

class [[nodiscard]] RenderingEngine final
{
public:
    RenderingEngine(
        dgm::Window& window,
        dgm::ResourceManager& resmgr,
        const VideoSettings& settings,
        const StringProvider& strings,
        Scene& scene,
        const TiledLevel& level,
        const GameConfig& config) noexcept;

public:
    void update(const dgm::Time& time);

    void draw(bool paused);

    void renderWorld();

    void
    renderMagnetLine(const sf::Vector2f& joePos, const sf::Vector2f& direction);

    void renderHUD();

    void setJoeIdleState();

private:
    dgm::Window& window;
    const VideoSettings& settings;
    const StringProvider& strings;
    Scene& scene;
    dgm::TextureAtlas atlas;
    dgm::AnimationStates ballAnimationStates;
    dgm::AnimationStates magnetLineAnimationStates;
    dgm::Clip tileset;

    BoxDebugRenderer boxDebugRenderer;
    dgm::Camera backgroundCamera;
    dgm::Camera worldCamera;
    dgm::Camera hudCamera;
    FpsCounter fpsCounter;
    SimpleAnimation animation;
    std::string joeSkinName;

    sf::Text text;
    dgm::TileMap tileMap;
    sf::Sprite sprite;
    sf::Sprite line;
    sf::CircleShape spriteOutline;
    sf::Sprite background;

    sf::Time timeToBlink = sf::Time::Zero;
    dgm::Animation joeAnimation;
};
