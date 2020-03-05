#pragma once
#include <GGEngine/Engine/Modules/World/Components/PhysicsComponent.h>
#include <GGEngine/Engine/Modules/World/Components/ShapeComponent.h>
#include <GGEngine/Engine/Modules/World/Components/TransformComponent.h>
#include <GGEngine/Engine/Modules/World/Systems/System.h>
#include <random>

struct PlayableTag {};

class InteractionSystem : public System {
public:
    virtual void update(epp::EntityManager& entMgr, float dt) override;

private:
    epp::Selection<ShapeComponent, PhysicsComponent, TransformComponent, PlayableTag> playableEnt;
};