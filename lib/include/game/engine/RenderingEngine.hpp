#pragma once

#include "game/Scene.hpp"
#include "misc/FpsCounter.hpp"
#include <DGM/dgm.hpp>

class [[nodiscard]] RenderingEngine final : public b2Draw
{
public:
    RenderingEngine(
        dgm::Window& window,
        dgm::ResourceManager& resmgr,
        Scene& scene) noexcept
        : window(window)
        , scene(scene)
        , text(resmgr.get<sf::Font>("ChunkFive-Regular.ttf"))
        , sprite(resmgr.get<sf::Texture>("joe.png"))
    {
        sprite.setOrigin(sf::Vector2f { sprite.getTexture().getSize() / 2u });
        spriteOutline.setRadius(sprite.getOrigin().x);
        spriteOutline.setOrigin(sprite.getOrigin());
        spriteOutline.setOutlineThickness(3.f);
        scene.world->SetDebugDraw(this);
        this->SetFlags(b2Draw::e_shapeBit);
    }

    RenderingEngine(RenderingEngine&&) = delete;
    RenderingEngine(const RenderingEngine&) = delete;

public:
    void update(const dgm::Time& time);

    void draw(dgm::Window& window);

    /// Draw a closed polygon provided in CCW order.
    void DrawPolygon(
        const b2Vec2* vertices,
        int32 vertexCount,
        const b2Color& color) override;

    /// Draw a solid closed polygon provided in CCW order.
    void DrawSolidPolygon(
        const b2Vec2* vertices,
        int32 vertexCount,
        const b2Color& color) override;

    void DrawCircle(
        const b2Vec2& center, float radius, const b2Color& color) override;

    /// Draw a solid circle.
    void DrawSolidCircle(
        const b2Vec2& center,
        float radius,
        const b2Vec2& axis,
        const b2Color& color) override;

    /// Draw a line segment.
    void DrawSegment(
        const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override {};

    /// Draw a transform. Choose your own length scale.
    /// @param xf a transform.
    void DrawTransform(const b2Transform& xf) override {};

    /// Draw a point.
    void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override {
    };

private:
    static sf::Color boxToSfColor(b2Color color)
    {
        return sf::Color(color.r * 255, color.g * 255, color.b * 255, 128);
    }

private:
    dgm::Window& window;
    Scene& scene;
    FpsCounter fpsCounter;
    sf::Text text;

    sf::Sprite sprite;
    sf::CircleShape spriteOutline;
};
