#include <GGEngine/Engine/Modules/World/Systems/CollisionSystem/CollisionSystem.h>


void CollisionSystem::updateGrid(epp::EntityManager& entMgr)
{
    // grid.clear();
    // for (auto it = entities.begin(); it != entities.end();) {
    //     auto& transfC = it.getComponent<TransformComponent>();
    //     auto& shapeC = it.getComponent<ShapeComponent>();
    //     if (transfC.getPosition().y >= 1000.f) {
    //         it = entMgr.destroy(it);
    //     }
    //     else {
    //         grid.insert(*it, transfC, shapeC);
    //         ++it;
    //     }
    // }
}

void CollisionSystem::findCollisions(epp::EntityManager& entMgr)
{
    // for (auto it = entities.begin(); it != entities.end(); ++it)
    //     it.getComponent<ShapeComponent>().getShape().setTransform(it.getComponent<TransformComponent>().getTransform());

    // for (auto it = entities.begin(); it != entities.end(); ++it) {
    //     auto& shapeC = it.getComponent<ShapeComponent>();

    //     for (auto const& vecPtr : grid.queryFor(it.getComponent<TransformComponent>().getPosition(), shapeC.getShape().getAABB()))
    //         for (auto otherEnt : *vecPtr) {
    //             if (otherEnt == *it)
    //                 continue;

    //             auto& shapeC2 = entMgr.componentOf<ShapeComponent>(otherEnt);
    //             c2Manifold manifold = shapeC.getShape().collisionManifold(shapeC2.getShape());
    //             if (manifold.count) {
    //                 // todo
    //             }
    //         }
    // }
}