#pragma once

#include <DGM/dgm.hpp>
#include <array>
#include <box2d/box2d.h>
#include <memory>

using PhysicsWorld = std::unique_ptr<b2World>;
using PhysicsBody = b2Body&;
using PhysicsCollider = b2Fixture;

struct DynamicBodyProperties
{
    float density = 1.0f;
    float friction = 0.5f;
    float restitution = 0.2f; // Bounciness
};

struct SensorProperties
{
    unsigned value = 0;
};

class Box2D
{
public:
    static PhysicsWorld createWorld();

    static PhysicsBody createBody(
        PhysicsWorld& world, b2Vec2 position, b2BodyType type = b2_staticBody);

    static PhysicsBody createStaticBox(
        PhysicsWorld& world,
        b2Vec2 position,
        b2Vec2 size,
        std::optional<SensorProperties> sensorProperties = {});

    static PhysicsBody
    createStaticTriangle(PhysicsWorld& world, std::array<b2Vec2, 3> vertices);

    static PhysicsBody createDynamicBall(
        PhysicsWorld& world,
        b2Vec2 position,
        float radius,
        const DynamicBodyProperties& properties = {});
};

class [[nodiscard]] BoxDebugRenderer final : public b2Draw
{
public:
    BoxDebugRenderer(dgm::Window& window) noexcept : window(window) {}

    void DrawPolygon(
        const b2Vec2* vertices,
        int32 vertexCount,
        const b2Color& color) override;

    void DrawSolidPolygon(
        const b2Vec2* vertices,
        int32 vertexCount,
        const b2Color& color) override;

    void DrawCircle(
        const b2Vec2& center, float radius, const b2Color& color) override;

    void DrawSolidCircle(
        const b2Vec2& center,
        float radius,
        const b2Vec2& axis,
        const b2Color& color) override;

    void DrawSegment(const b2Vec2&, const b2Vec2&, const b2Color&) override {};

    void DrawTransform(const b2Transform&) override {};

    void DrawPoint(const b2Vec2&, float, const b2Color&) override {};

private:
    dgm::Window& window;
};
