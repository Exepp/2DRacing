#pragma once
#include <GGEngine/Engine/Modules/World/Components/PhysicsComponent.h>
#include <GGEngine/Engine/Modules/World/Systems/CollisionSystem/CollisionSystem.h>
#include <GGEngine/Engine/Modules/World/Systems/System.h>

class PhysicsSystem : public System {
    using DynamicSelect_t = epp::Selection<TransformComponent, ShapeComponent, PhysicsComponent>;
    using StaticSelect_t = epp::Selection<TransformComponent, ShapeComponent, PhysicsComponent, StaticObjectTag>;
    using DynamicSelIterator_t = DynamicSelect_t::Iterator_t;

public:
    virtual void update(epp::EntityManager& entMgr, float dt) override;

    CollisionSystem const& getCollisionSystem() const { return cs; }

private:
    void resolveCollisions(epp::EntityManager& entMgr);
    void applyGravity(float dt);
    void resolveVelocities(float dt);

private:
    DynamicSelect_t dynamicEnts = DynamicSelect_t(epp::IdOfL<StaticObjectTag>());
    StaticSelect_t staticEnts;
    CollisionSystem cs;
};
