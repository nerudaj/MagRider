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

namespace box
{
    const static inline b2Vec2 GRAVITY = b2Vec2(0.0f, 9.8f);

    static PhysicsWorld createWorld()
    {
        return std::make_unique<b2World>(GRAVITY);
    }

    static PhysicsBody createBody(
        PhysicsWorld& world, b2Vec2 position, b2BodyType type = b2_staticBody)
    {
        b2BodyDef bodyDef;
        bodyDef.position.Set(position.x, position.y);
        bodyDef.type = type;
        return *world->CreateBody(&bodyDef);
    }

    static PhysicsBody createStaticBox(
        PhysicsWorld& world,
        b2Vec2 position,
        b2Vec2 size,
        std::optional<SensorProperties> sensorProperties = {})
    {
        auto&& body = createBody(world, position);
        b2PolygonShape boxShape;
        boxShape.SetAsBox(size.x / 2.0f, size.y / 2.0f);
        if (!sensorProperties)
        {

            body.CreateFixture(&boxShape, 0.0f);
        }
        else
        {
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &boxShape;
            fixtureDef.isSensor = true;
            fixtureDef.userData.pointer = sensorProperties->value;
            body.CreateFixture(&fixtureDef);
        }
        return body;
    }

    static PhysicsBody
    createStaticTriangle(PhysicsWorld& world, std::array<b2Vec2, 3> vertices)
    {
        auto&& body = createBody(world, b2Vec2_zero);

        b2PolygonShape shape;
        shape.Set(vertices.data(), static_cast<int32>(vertices.size()));
        body.CreateFixture(&shape, 0.f);
        return body;
    }

    static PhysicsBody createDynamicBall(
        PhysicsWorld& world,
        b2Vec2 position,
        float radius,
        const DynamicBodyProperties& properties = {})
    {
        auto&& body = createBody(world, position, b2_dynamicBody);

        b2CircleShape circleShape;
        circleShape.m_radius = radius;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &circleShape;
        fixtureDef.density = properties.density;
        fixtureDef.friction = properties.friction;
        fixtureDef.restitution = properties.restitution;

        body.CreateFixture(&fixtureDef);
        return body;
    }
} // namespace box

namespace box
{
    static inline void updateWorld(PhysicsWorld& world, const dgm::Time& time)
    {
        world->Step(time.getDeltaTime(), 6, 2);
    }
} // namespace box

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
