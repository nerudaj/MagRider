#pragma once

#include "game/Box2d.hpp"
#include "game/Scene.hpp"
#include "game/TiledLevel.hpp"

namespace tiled
{
    class FiniteMapModel;
}

class SceneBuilder
{
public:
    static TiledLevel convertToTiledLevel(const tiled::FiniteMapModel& map);

    static void generateColliders(PhysicsWorld& world, const TiledLevel& level);

    static std::vector<Magnet> getMagnets(const TiledLevel& level);

    static Scene buildScene(const TiledLevel& level);
};
