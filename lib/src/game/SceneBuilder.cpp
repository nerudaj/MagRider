#include "game/SceneBuilder.hpp"
#include "filesystem/models/TiledModels.hpp"
#include "misc/Compatibility.hpp"
#include "misc/CoordConverter.hpp"
#include "types/SemanticTypes.hpp"

TiledLevel SceneBuilder::convertToTiledLevel(const tiled::FiniteMapModel& map)
{
    auto&& tileLayers =
        map.layers
        | std::views::filter(
            [](const tiled::LayerModel& layer)
            { return std::holds_alternative<tiled::TileLayerModel>(layer); })
        | std::views::transform(
            [](const tiled::LayerModel& layer)
            { return std::get<tiled::TileLayerModel>(layer); });

    auto&& objectLayers =
        map.layers
        | std::views::filter(
            [](const tiled::LayerModel& layer)
            { return std::holds_alternative<tiled::ObjectGroupModel>(layer); })
        | std::views::transform(
            [](const tiled::LayerModel& layer)
            { return std::get<tiled::ObjectGroupModel>(layer); });

    auto&& toTileLayer = [](const tiled::TileLayerModel& model)
    {
        return TileLayer {
            .id = model.id,
            .tiles = model.data
                     | std::views::transform(
                         [&](int tile) -> Tile
                         {
                             if (tile == 0) return Tile::Empty;
                             return static_cast<Tile>(tile);
                         })
                     | uniranges::to<std::vector>(),
        };
    };

    auto&& toObjectLayer = [](const tiled::ObjectGroupModel& model)
    {
        return ObjectLayer {
            .positions =
                model.objects
                | std::views::transform(
                    [](const tiled::ObjectModel& objectModel)
                    { return sf::Vector2f(objectModel.x, objectModel.y); })
                | uniranges::to<std::vector>(),
        };
    };

    return TiledLevel {
        .width = map.width,
        .height = map.height,
        .tileWidth = map.tilewidth,
        .tileHeight = map.tileheight,
        .tileLayers = tileLayers | std::views::transform(toTileLayer)
                      | uniranges::to<std::vector<TileLayer>>(),
        .objectLayers = objectLayers | std::views::transform(toObjectLayer)
                        | uniranges::to<std::vector<ObjectLayer>>(),
    };
}

void SceneBuilder::generateColliders(
    PhysicsWorld& world, const TiledLevel& level)
{
    // Create collision boxes
    for (unsigned y = 0; y < level.height; ++y)
    {
        for (unsigned x = 0; x < level.width; ++x)
        {
            const unsigned idx = y * level.width + x;
            const auto tile = level.tileLayers[0].tiles[idx];
            const float fx = static_cast<float>(x);
            const float fy = static_cast<float>(y);

            if (tile == Tile::Block || tile == Tile::MagNeg
                || tile == Tile::MagPlus)
            {
                Box2D::createStaticBox(
                    world, b2Vec2(fx + 0.5f, fy + 0.5f), b2Vec2(1.f, 1.f));
            }
            else if (tile == Tile::Finish)
            {
                Box2D::createStaticBox(
                    world,
                    b2Vec2(fx + 0.5f, fy + 0.5f),
                    b2Vec2(0.4f, 0.4f),
                    SensorProperties {
                        .value = FINISH,
                    });
            }
            else if (tile == Tile::FloorUp45)
            {
                Box2D::createStaticTriangle(
                    world,
                    {
                        b2Vec2(fx, fy + 1.f),
                        b2Vec2(fx + 1.f, fy),
                        b2Vec2(fx + 1.f, fy + 1.f),
                    });
            }
            else if (tile == Tile::FloorDown45)
            {
                Box2D::createStaticTriangle(
                    world,
                    {
                        b2Vec2(fx, fy),
                        b2Vec2(fx + 1.f, fy + 1.f),
                        b2Vec2(fx, fy + 1.f),
                    });
            }
            else if (tile == Tile::CeilDown45)
            {
                Box2D::createStaticTriangle(
                    world,
                    {
                        b2Vec2(fx, fy),
                        b2Vec2(fx + 1.f, fy),
                        b2Vec2(fx + 1.f, fy + 1.f),
                    });
            }
            else if (tile == Tile::CeilUp45)
            {
                Box2D::createStaticTriangle(
                    world,
                    {
                        b2Vec2(fx, fy),
                        b2Vec2(fx + 1.f, fy),
                        b2Vec2(fx, fy + 1.f),
                    });
            }
            // 60 degree triangles
            else if (tile == Tile::FloorUp60Small)
            {
                Box2D::createStaticTriangle(
                    world,
                    {
                        b2Vec2(fx, fy + 1.f),
                        b2Vec2(fx + 2.f, fy),
                        b2Vec2(fx + 2.f, fy + 1.f),
                    });
            }
            else if (tile == Tile::FloorUp120Small)
            {
                Box2D::createStaticTriangle(
                    world,
                    {
                        b2Vec2(fx, fy + 2.f),
                        b2Vec2(fx + 1.f, fy),
                        b2Vec2(fx + 1.f, fy + 2.f),
                    });
            }
            else if (tile == Tile::FloorDown60Big)
            {
                Box2D::createStaticTriangle(
                    world,
                    {
                        b2Vec2(fx, fy),
                        b2Vec2(fx + 2.f, fy + 1.f),
                        b2Vec2(fx, fy + 1.f),
                    });
            }
            else if (tile == Tile::FloorDown120Small)
            {
                Box2D::createStaticTriangle(
                    world,
                    {
                        b2Vec2(fx, fy),
                        b2Vec2(fx + 1.f, fy + 2.f),
                        b2Vec2(fx, fy + 2.f),
                    });
            }
            else if (tile == Tile::CeilDown60Small)
            {
                Box2D::createStaticTriangle(
                    world,
                    {
                        b2Vec2(fx, fy),
                        b2Vec2(fx + 2.f, fy),
                        b2Vec2(fx + 2.f, fy + 1.f),
                    });
            }
            else if (tile == Tile::CeilDown120Big)
            {
                Box2D::createStaticTriangle(
                    world,
                    {
                        b2Vec2(fx, fy),
                        b2Vec2(fx + 1.f, fy),
                        b2Vec2(fx + 1.f, fy + 2.f),
                    });
            }
            else if (tile == Tile::CeilUp60Big)
            {
                Box2D::createStaticTriangle(
                    world,
                    {
                        b2Vec2(fx, fy),
                        b2Vec2(fx + 2.f, fy),
                        b2Vec2(fx, fy + 1.f),
                    });
            }
            else if (tile == Tile::CeilUp120Big)
            {
                Box2D::createStaticTriangle(
                    world,
                    {
                        b2Vec2(fx, fy),
                        b2Vec2(fx + 1.f, fy),
                        b2Vec2(fx, fy + 2.f),
                    });
            }
            // spikes
            else if (tile == Tile::SpikeUp)
            {
                Box2D::createStaticBox(
                    world,
                    b2Vec2(fx + 0.5f, fy + 0.75f),
                    b2Vec2(1.f, 0.5f),
                    SensorProperties {
                        .value = SPIKE,
                    });
            }
            else if (tile == Tile::SpikeLeft)
            {
                Box2D::createStaticBox(
                    world,
                    b2Vec2(fx + 0.75f, fy + 0.5f),
                    b2Vec2(0.5f, 1.f),
                    SensorProperties {
                        .value = SPIKE,
                    });
            }
            else if (tile == Tile::SpikeRight)
            {
                Box2D::createStaticBox(
                    world,
                    b2Vec2(fx + 0.25f, fy + 0.5f),
                    b2Vec2(0.5f, 1.f),
                    SensorProperties {
                        .value = SPIKE,
                    });
            }
            else if (tile == Tile::SpikeDown)
            {
                Box2D::createStaticBox(
                    world,
                    b2Vec2(fx + 0.5f, fy + 0.25f),
                    b2Vec2(1.f, 0.5f),
                    SensorProperties {
                        .value = SPIKE,
                    });
            }
        }
    }
}

std::vector<Magnet> SceneBuilder::getMagnets(const TiledLevel& level)
{
    auto&& magnets = std::vector<Magnet>();

    for (unsigned y = 0; y < level.height; ++y)
    {
        for (unsigned x = 0; x < level.width; ++x)
        {
            const unsigned idx = y * level.width + x;
            const auto tile = level.tileLayers[0].tiles[idx];
            const float fx = static_cast<float>(x) + 0.5f;
            const float fy = static_cast<float>(y) + 0.5f;

            if (tile == Tile::MagNeg)
                magnets.push_back(Magnet {
                    .position = { fx, fy },
                    .polarity = 1,
                });
            else if (tile == Tile::MagPlus)
                magnets.push_back(Magnet {
                    .position = { fx, fy },
                    .polarity = 2,
                });
        }
    }

    return magnets;
}

Scene SceneBuilder::buildScene(const TiledLevel& level)
{
    auto world = Box2D::createWorld();
    generateColliders(world, level);

    auto& joeBody = Box2D::createDynamicBall(
        world,
        CoordConverter::screenToWorld(
            level.objectLayers.front().positions.front()),
        0.5f,
        DynamicBodyProperties {
            .density = 0.5f,
            .restitution = 0.4f,
        });

    auto listener = std::make_unique<SpikeContactListener>();
    world->SetContactListener(listener.get());

    return Scene {
        .world = std::move(world),
        .joe = joeBody,
        .magnets = getMagnets(level),
        .contactListener = std::move(listener),
    };
}
