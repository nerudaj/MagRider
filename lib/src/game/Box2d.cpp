#include "game/Box2d.hpp"
#include "misc/CoordConverter.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

static sf::Color boxToSfColor(b2Color color)
{
    return sf::Color(color.r * 255, color.g * 255, color.b * 255, 128);
}

void BoxDebugRenderer::DrawPolygon(
    const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    sf::ConvexShape shape(vertexCount);

    for (unsigned i = 0; i < vertexCount; ++i)
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

    for (unsigned i = 0; i < vertexCount; ++i)
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
    const b2Vec2& center,
    float radius,
    const b2Vec2& axis,
    const b2Color& color)
{
    sf::CircleShape circle;
    circle.setRadius(CoordConverter::worldToScreen(radius));
    circle.setOrigin({ circle.getRadius(), circle.getRadius() });
    circle.setPosition(CoordConverter::worldToScreen(center));
    circle.setFillColor(boxToSfColor(color));
    window.draw(circle);
}
