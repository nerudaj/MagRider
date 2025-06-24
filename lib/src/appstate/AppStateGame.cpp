#include "appstate/AppStateGame.hpp"
#include "appstate/AppStatePause.hpp"
#include "appstate/Messaging.hpp"
#include "loaders/models/TiledModels.hpp"
#include "types/Overloads.hpp"

void AppStateGame::input()
{
    if (dic.input.isBackButtonPressed())
    {
        app.pushState<AppStatePause>(dic, settings);
    }

    while (const auto event = app.window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            app.exit();
        }
    }
}

void AppStateGame::update()
{
    gameRulesEngine.update(app.time);
    renderingEngine.update(app.time);

    gameEvents.processEvents(
        [&](const DummyGameEvent& e)
        {
            sound.setBuffer(dic.resmgr.get<sf::SoundBuffer>(e.soundName));
            sound.play();
        });

    if (scene.contactListener->died)
    {
        app.popState();
    }
}

void AppStateGame::draw()
{
    renderingEngine.draw(app.window);
}

void AppStateGame::restoreFocusImpl(const std::string& msg)
{
    if (auto message = Messaging::deserialize(msg))
    {
        std::visit([&](PopIfNotMenu&) { app.popState(msg); }, *message);
    }
}

dgm::Camera createFullscreenCamera(
    const dgm::Window& window, const sf::Vector2u& desiredResolution)
{
    auto&& viewport = sf::FloatRect {
        { 0.f, 0.f },
        { 1.f, 1.f },
    };

    const auto& desiredResolutionF = sf::Vector2f(desiredResolution);
    const auto& currentResolution = sf::Vector2f(window.getSize());
    const auto& desiredAspectRatio =
        desiredResolutionF.x / desiredResolutionF.y;
    const auto& currentAspectRatio = currentResolution.x / currentResolution.y;

    if (currentAspectRatio > desiredAspectRatio)
    {
        // Narrow desired into wider current
        viewport.size.x = desiredResolutionF.x / currentResolution.x;
        viewport.position.x = (1.f - viewport.size.x) / 2.f;
    }
    else if (currentAspectRatio < desiredAspectRatio)
    {
        // Wider desired into narrower current
        viewport.size.y = desiredResolutionF.y / currentResolution.y;
        viewport.position.y = (1.f - viewport.size.y) / 2.f;
    }

    return dgm::Camera(viewport, desiredResolutionF);
}

#include "box2d/box2d.h"
#include "misc/CoordConverter.hpp"

using PhysicsWorld = std::unique_ptr<b2World>;
using PhysicsBody = b2Body&;
using PhysicsCollider = b2Fixture;

struct DynamicBodyProperties
{
    float density = 1.0f;
    float friction = 0.5f;
    float restitution = 0.2f; // Bounciness
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
        bool isSensor = false)
    {
        auto&& body = createBody(world, position);
        b2PolygonShape boxShape;
        boxShape.SetAsBox(size.x / 2.0f, size.y / 2.0f);
        if (!isSensor)
        {

            body.CreateFixture(&boxShape, 0.0f);
        }
        else
        {
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &boxShape;
            fixtureDef.isSensor = true;
            body.CreateFixture(&fixtureDef);
        }
        return body;
    }

    static PhysicsBody
    createStaticTriangle(PhysicsWorld& world, std::array<b2Vec2, 3> vertices)
    {
        auto&& body = createBody(world, b2Vec2_zero);

        b2PolygonShape shape;
        shape.Set(vertices.data(), vertices.size());
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

Scene AppStateGame::buildScene(
    const dgm::ResourceManager& resmgr, const dgm::Window& window)
{
    const auto& map = resmgr.get<tiled::FiniteMapModel>("intro.json");
    auto spawn = sf::Vector2f {};
    auto tilemap = dgm::TileMap(
        resmgr.get<sf::Texture>("set.png"),
        resmgr.get<dgm::Clip>("set.png.clip"));

    const tiled::TileLayerModel* tileModel;
    for (auto&& layer : map.layers)
    {
        std::visit(
            overloads {
                [&](const tiled::TileLayerModel& model)
                {
                    tileModel = &model;
                    tilemap.build(
                        { map.tilewidth, map.tileheight },
                        model.data
                            | std::views::transform(
                                [&](int tile)
                                { return tile == 0 ? 12 : tile - 1; })
                            | uniranges::to<std::vector>(),
                        { map.width, map.height });
                },
                [&](const tiled::ObjectGroupModel& model)
                {
                    spawn = sf::Vector2f {
                        model.objects.front().x,
                        model.objects.front().y,
                    };
                },
            },
            layer);
    }

    // Somehow build whole box2d world
    auto world = box::createWorld();

    auto redPositions = std::vector<sf::Vector2f> {};
    auto bluePositions = std::vector<sf::Vector2f> {};
    // Create collision boxes
    for (unsigned y = 0; y < map.height; ++y)
    {
        for (unsigned x = 0; x < map.width; ++x)
        {
            const unsigned idx = y * map.width + x;
            if (tileModel->data[idx] == 0) continue;

            const auto tile = tileModel->data[idx] - 1;
            const float fx = static_cast<float>(x);
            const float fy = static_cast<float>(y);

            if (tile == 18 || tile == 25 || tile == 30)
            {
                if (tile == 18)
                    redPositions.push_back({ fx + 0.5f, fy + 0.5f });
                else if (tile == 30)
                    bluePositions.push_back({ fx + 0.5f, fy + 0.5f });

                box::createStaticBox(
                    world, b2Vec2(fx + 0.5f, fy + 0.5f), b2Vec2(1.f, 1.f));
            }
            else if (tile == 14)
            {
                box::createStaticTriangle(
                    world,
                    {
                        b2Vec2(fx, fy + 1.f),
                        b2Vec2(fx + 1.f, fy),
                        b2Vec2(fx + 1.f, fy + 1.f),
                    });
            }
            // 45 degree triangles
            else if (tile == 15)
            {
                box::createStaticTriangle(
                    world,
                    {
                        b2Vec2(fx, fy),
                        b2Vec2(fx + 1.f, fy + 1.f),
                        b2Vec2(fx, fy + 1.f),
                    });
            }
            else if (tile == 20)
            {
                box::createStaticTriangle(
                    world,
                    {
                        b2Vec2(fx, fy),
                        b2Vec2(fx + 1.f, fy),
                        b2Vec2(fx + 1.f, fy + 1.f),
                    });
            }
            else if (tile == 21)
            {
                box::createStaticTriangle(
                    world,
                    {
                        b2Vec2(fx, fy),
                        b2Vec2(fx + 1.f, fy),
                        b2Vec2(fx, fy + 1.f),
                    });
            }
            // 60 degree triangles
            else if (tile == 0)
            {
                box::createStaticTriangle(
                    world,
                    {
                        b2Vec2(fx, fy + 1.f),
                        b2Vec2(fx + 2.f, fy),
                        b2Vec2(fx + 2.f, fy + 1.f),
                    });
            }
            else if (tile == 4)
            {
                box::createStaticTriangle(
                    world,
                    {
                        b2Vec2(fx, fy + 2.f),
                        b2Vec2(fx + 1.f, fy),
                        b2Vec2(fx + 1.f, fy + 2.f),
                    });
            }
            else if (tile == 2)
            {
                box::createStaticTriangle(
                    world,
                    {
                        b2Vec2(fx, fy),
                        b2Vec2(fx + 2.f, fy + 1.f),
                        b2Vec2(fx, fy + 1.f),
                    });
            }
            else if (tile == 5)
            {
                box::createStaticTriangle(
                    world,
                    {
                        b2Vec2(fx, fy),
                        b2Vec2(fx + 1.f, fy + 2.f),
                        b2Vec2(fx, fy + 2.f),
                    });
            }
            else if (tile == 6)
            {
                box::createStaticTriangle(
                    world,
                    {
                        b2Vec2(fx, fy),
                        b2Vec2(fx + 2.f, fy),
                        b2Vec2(fx + 2.f, fy + 1.f),
                    });
            }
            else if (tile == 16)
            {
                box::createStaticTriangle(
                    world,
                    {
                        b2Vec2(fx, fy),
                        b2Vec2(fx + 1.f, fy),
                        b2Vec2(fx + 1.f, fy + 2.f),
                    });
            }
            else if (tile == 8)
            {
                box::createStaticTriangle(
                    world,
                    {
                        b2Vec2(fx, fy),
                        b2Vec2(fx + 2.f, fy),
                        b2Vec2(fx, fy + 1.f),
                    });
            }
            else if (tile == 17)
            {
                box::createStaticTriangle(
                    world,
                    {
                        b2Vec2(fx, fy),
                        b2Vec2(fx + 1.f, fy),
                        b2Vec2(fx, fy + 2.f),
                    });
            }
            // spikes
            else if (tile == 19)
            {
                box::createStaticBox(
                    world,
                    b2Vec2(fx + 0.5f, fy + 0.75f),
                    b2Vec2(1.f, 0.5f),
                    "isSensor"_true);
            }
            else if (tile == 24)
            {
                box::createStaticBox(
                    world,
                    b2Vec2(fx + 0.75f, fy + 0.5f),
                    b2Vec2(0.5f, 1.f),
                    "isSensor"_true);
            }
            else if (tile == 26)
            {
                box::createStaticBox(
                    world,
                    b2Vec2(fx + 0.25f, fy + 0.5f),
                    b2Vec2(0.5f, 1.f),
                    "isSensor"_true);
            }
            else if (tile == 31)
            {
                box::createStaticBox(
                    world,
                    b2Vec2(fx + 0.5f, fy + 0.25f),
                    b2Vec2(1.f, 0.5f),
                    "isSensor"_true);
            }
        }
    }

    // Create joe
    auto& joeBody = box::createDynamicBall(
        world,
        CoordConverter::screenToWorld(spawn),
        0.5f,
        DynamicBodyProperties {
            .density = 0.6f,
            .restitution = 0.4f,
        });

    auto listener = std::make_unique<SpikeContactListener>();
    world->SetContactListener(listener.get());

    return Scene {
        .position = spawn,
        .tileMap = std::move(tilemap),
        .camera = createFullscreenCamera(window, { 640u, 360u }),
        .hudCamera = dgm::Camera(
            sf::FloatRect { { 0.f, 0.f }, { 1.f, 1.f } },
            sf::Vector2f(window.getSize())),
        .contactListener = std::move(listener),
        .world = std::move(world),
        .joe = joeBody,
        .redMagnetPositions = std::move(redPositions),
        .blueMagnetPositions = std::move(bluePositions),
    };
}
