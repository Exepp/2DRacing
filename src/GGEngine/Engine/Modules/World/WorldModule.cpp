#include <GGEngine/Engine/Modules/World/WorldModule.h>
#include <GGEngine/Engine/Utilities/Time/Clock.h>
#include <GGEngine/Game.h>


WorldModule::WorldModule()
{
    interactionSystem = std::make_unique<InteractionSystem>();
    physicsSystem = std::make_unique<PhysicsSystem>();
    displaySystem = std::make_unique<DisplaySystem>(*physicsSystem);

    epp::Archetype arche(epp::IdOf<DisplayComponent, ShapeComponent, TransformComponent, PhysicsComponent, PlayableTag>());
    mgr.spawn(arche, [&](epp::EntitySpawner::Creator&& creator) {
        auto& shC = creator.constructed<ShapeComponent>();
        auto& phC = creator.constructed<PhysicsComponent>();
        auto& trC = creator.constructed<TransformComponent>();
        float a = 4.5f;
        float b = 2.f;
        shC.takeShape<PolygonShape>().setAsCar(Vec2f(a, b));
        phC.setMass(a * b * 300);
        phC.moi = 1.f / 12.f * phC.mass * (a * a + b * b);
        phC.moiInv = 1.f / phC.moi;
        trC.setPosition({ 0, 0 });
        // phC.velocity = Vec2f(10, 0.f);
        // phC.wheelAngVel[0] = -M_PI * 2.f;
    });
}

void WorldModule::update()
{
    interactionSystem->update(mgr, SimulationClock::timestep);
    physicsSystem->update(mgr, SimulationClock::timestep);
    displaySystem->update(mgr, SimulationClock::timestep);
}