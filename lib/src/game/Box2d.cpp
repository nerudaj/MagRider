#include "game/Box2d.hpp"
#include "misc/CoordConverter.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

static sf::Color boxToSfColor(b2Color color)
{
    return sf::Color(
        static_cast<uint8_t>(color.r * 255),
        static_cast<uint8_t>(color.g * 255),
        static_cast<uint8_t>(color.b * 255),
        128);
}

PhysicsWorld Box2D::createWorld()
{
    const auto GRAVITY = b2Vec2(0.0f, 9.8f);
    return std::make_unique<b2World>(GRAVITY);
}

PhysicsBody
Box2D::createBody(PhysicsWorld& world, b2Vec2 position, b2BodyType type)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set(position.x, position.y);
    bodyDef.type = type;
    return *world->CreateBody(&bodyDef);
}

PhysicsBody Box2D::createStaticBox(
    PhysicsWorld& world,
    b2Vec2 position,
    b2Vec2 size,
    std::optional<SensorProperties> sensorProperties)
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

PhysicsBody
Box2D::createStaticTriangle(PhysicsWorld& world, std::array<b2Vec2, 3> vertices)
{
    auto&& body = createBody(world, b2Vec2_zero);

    b2PolygonShape shape;
    shape.Set(vertices.data(), static_cast<int32>(vertices.size()));
    body.CreateFixture(&shape, 0.f);
    return body;
}

PhysicsBody Box2D::createDynamicBall(
    PhysicsWorld& world,
    b2Vec2 position,
    float radius,
    const DynamicBodyProperties& properties)
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

void BoxDebugRenderer::DrawPolygon(
    const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    sf::ConvexShape shape(vertexCount);

    for (size_t i = 0; i < static_cast<size_t>(vertexCount); ++i)
    {
        shape.setPoint(i, CoordConverter::worldToScreen(vertices[i]));
    }

    shape.setOutlineColor(boxToSfColor(color));
    shape.setOutlineThickness(1.f);
    window.draw(shape);
}

void BoxDebugRenderer::DrawSolidPolygon(
    const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    sf::ConvexShape shape(vertexCount);

    for (size_t i = 0; i < static_cast<size_t>(vertexCount); ++i)
    {
        shape.setPoint(i, CoordConverter::worldToScreen(vertices[i]));
    }

    shape.setFillColor(boxToSfColor(color));
    window.draw(shape);
}

void BoxDebugRenderer::DrawCircle(
    const b2Vec2& center, float radius, const b2Color& color)
{
    sf::CircleShape circle;
    circle.setRadius(CoordConverter::worldToScreen(radius));
    circle.setOrigin({ circle.getRadius(), circle.getRadius() });
    circle.setPosition(CoordConverter::worldToScreen(center));
    circle.setOutlineColor(boxToSfColor(color));
    circle.setOutlineThickness(1.f);
    window.draw(circle);
}

void BoxDebugRenderer::DrawSolidCircle(
    const b2Vec2& center, float radius, const b2Vec2&, const b2Color& color)
{
    sf::CircleShape circle;
    circle.setRadius(CoordConverter::worldToScreen(radius));
    circle.setOrigin({ circle.getRadius(), circle.getRadius() });
    circle.setPosition(CoordConverter::worldToScreen(center));
    circle.setFillColor(boxToSfColor(color));
    window.draw(circle);
}
