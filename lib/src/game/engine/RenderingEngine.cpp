#include "game/engine/RenderingEngine.hpp"
#include "misc/CoordConverter.hpp"

void RenderingEngine::update(const dgm::Time& time)
{
    fpsCounter.update(time.getDeltaTime());
}

static sf::Vector2f operator-(const b2Vec2& a, sf::Vector2f& b)
{
    return {
        a.x - b.x,
        a.y - b.y,
    };
}

void RenderingEngine::draw(dgm::Window& _window)
{
    auto joePos = CoordConverter::worldToScreen(scene.joe.GetPosition());

    scene.camera.setPosition(joePos);
    sprite.setPosition(joePos);
    sprite.setRotation(sf::radians(scene.joe.GetAngle()));
    spriteOutline.setPosition(joePos);

    if (scene.magnetPolarity == 0)
        spriteOutline.setOutlineColor(sf::Color::Transparent);
    else if (scene.magnetPolarity == 1)
        spriteOutline.setOutlineColor(sf::Color::Red);
    else if (scene.magnetPolarity == 2)
        spriteOutline.setOutlineColor(sf::Color::Blue);

    window.setViewFromCamera(scene.camera);

    window.draw(scene.tileMap);
    window.draw(spriteOutline);
    window.draw(sprite);

    if (scene.magnetPolarity != 0)
    {
        for (auto&& magnetPos : scene.redMagnetPositions)
        {
            const auto direction = scene.joe.GetPosition() - magnetPos;
            if (direction.length() < 6.f)
            {
                auto vertices = std::array {
                    sf::Vertex { .position = joePos },
                    sf::Vertex { magnetPos * 32.f },
                };
                window.getSfmlWindowContext().draw(
                    vertices.data(), 2u, sf::PrimitiveType::Lines);
            }
        }

        for (auto&& magnetPos : scene.blueMagnetPositions)
        {
            const auto direction = scene.joe.GetPosition() - magnetPos;
            if (direction.length() < 6.f)
            {
                auto vertices = std::array {
                    sf::Vertex { .position = joePos },
                    sf::Vertex { magnetPos * 32.f },
                };
                window.getSfmlWindowContext().draw(
                    vertices.data(), 2u, sf::PrimitiveType::Lines);
            }
        }
    }

    // scene.world->DebugDraw();

    window.setViewFromCamera(scene.hudCamera);

    if (!scene.playing)
    {
        text.setString("Press SPACE to start");
        text.setPosition(
            sf::Vector2f(window.getSize()) / 2.f
            - text.getGlobalBounds().size / 2.f);
        window.draw(text);
    }

    text.setPosition({ 10.f, 10.f });
    text.setString(fpsCounter.getText());
    window.draw(text);
}

void RenderingEngine::DrawPolygon(
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

void RenderingEngine::DrawSolidPolygon(
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

void RenderingEngine::DrawCircle(
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

void RenderingEngine::DrawSolidCircle(
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
