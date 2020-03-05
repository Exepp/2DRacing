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
        float a = 1.5f;
        float b = 0.75f;
        shC.takeShape<PolygonShape>().setAsBox(Vec2f(a, b));
        phC.setMass(a * b);
        phC.moi = 1.f / 12.f * a * b * (a * a + b * b);
        phC.moiInv = 1.f / phC.moi;
        // float r = 3.f;
        // shC.takeShape<CircleShape>().setRadius(r);
        // phC.setMass(PI_F * r * r);
        // phC.moi = 1.f / 2.f * PI_F * r * r * r * r;
        // phC.moiInv = 1.f / phC.moi;
        trC.setPosition({ 0, 0 });
    });
}

void WorldModule::update()
{
    interactionSystem->update(mgr, SimulationClock::getDeltaTime());
    physicsSystem->update(mgr, SimulationClock::getDeltaTime());
    displaySystem->update(mgr, SimulationClock::getDeltaTime());
}