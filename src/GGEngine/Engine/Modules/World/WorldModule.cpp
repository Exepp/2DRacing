#include <GGEngine/Engine/Modules/World/WorldModule.h>
#include <GGEngine/Engine/Utilities/Time/Clock.h>
#include <GGEngine/Game.h>


WorldModule::WorldModule()
{
    interactionSystem = std::make_unique<InteractionSystem>();
    physicsSystem = std::make_unique<PhysicsSystem>();
    displaySystem = std::make_unique<DisplaySystem>(*physicsSystem);


    float edgeSize = 500;

    epp::Archetype arche(epp::IdOf<DisplayComponent, ShapeComponent, TransformComponent, PhysicsComponent, StaticObjectTag>());


    mgr.spawn(arche, [&](epp::EntitySpawner::Creator&& creator) {
        creator.constructed<ShapeComponent>().takeShape<PolygonShape>().setAsBox({ 3 * edgeSize, edgeSize });
        creator.constructed<TransformComponent>().setPosition(Vec2f(0, edgeSize / 2.f - 5));
        creator.constructed<PhysicsComponent>().setMass(0);
    });

    mgr.spawn(arche, [&](epp::EntitySpawner::Creator&& creator) {
        creator.constructed<ShapeComponent>().takeShape<PolygonShape>().setAsBox({ 3 * edgeSize, edgeSize });
        creator.constructed<TransformComponent>().setPosition(Vec2f(0, -3 * edgeSize / 2.f));
        creator.constructed<PhysicsComponent>().setMass(0);
    });

    mgr.spawn(arche, [&](epp::EntitySpawner::Creator&& creator) {
        creator.constructed<ShapeComponent>().takeShape<PolygonShape>().setAsBox({ edgeSize, edgeSize });
        creator.constructed<TransformComponent>().setPosition(Vec2f(-edgeSize, -edgeSize / 2.f));
        creator.constructed<PhysicsComponent>().setMass(0);
    });

    mgr.spawn(arche, [&](epp::EntitySpawner::Creator&& creator) {
        creator.constructed<ShapeComponent>().takeShape<PolygonShape>().setAsBox({ edgeSize, edgeSize });
        creator.constructed<TransformComponent>().setPosition(Vec2f(edgeSize, -edgeSize / 2.f));
        creator.constructed<PhysicsComponent>().setMass(0);
    });

    arche.removeComponent<StaticObjectTag>();
    // for (int i = 0; i < 4e3; ++i)
    mgr.spawn(arche, [&](epp::EntitySpawner::Creator&& creator) {
        auto& shC = creator.constructed<ShapeComponent>();
        auto& phC = creator.constructed<PhysicsComponent>();
        auto& trC = creator.constructed<TransformComponent>();
        float a = 0.5f + 5.f;
        float b = 0.5f + 5.f;
        shC.takeShape<PolygonShape>().setAsBox(Vec2f(a, b));
        phC.setMass(a * b);
        phC.moi = 1.f / 12.f * a * b * (a * a + b * b);
        phC.moiInv = 1.f / phC.moi;
        // float r = 3.f;
        // shC.takeShape<CircleShape>().setRadius(r);
        // phC.setMass(PI_F * r * r);
        // phC.moi = 1.f / 2.f * PI_F * r * r * r * r;
        // phC.moiInv = 1.f / phC.moi;
        trC.setPosition({ -edgeSize / 2.f + rand() % int(edgeSize), -edgeSize + rand() % int(edgeSize) });
    });
}

void WorldModule::update()
{
    interactionSystem->update(mgr, SimulationClock::getDeltaTime());
    physicsSystem->update(mgr, SimulationClock::getDeltaTime());
    displaySystem->update(mgr, SimulationClock::getDeltaTime());
}