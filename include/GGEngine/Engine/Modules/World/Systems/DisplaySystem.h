#pragma once
#include <GGEngine/Engine/Modules/World/Components/DisplayComponent.h>
#include <GGEngine/Engine/Modules/World/Components/ShapeComponent.h>
#include <GGEngine/Engine/Modules/World/Components/TransformComponent.h>
#include <GGEngine/Engine/Modules/World/Systems/PhysicsSystem.h>
#include <GGEngine/Engine/Modules/World/Systems/System.h>


class DisplaySystem : public System {
public:
    DisplaySystem(PhysicsSystem const& phySys) : phySys(phySys) {}

    virtual void update(epp::EntityManager& entMgr, float dt) override;

private:
    void drawEntities();
    void drawCollisionGrid();

private:
    epp::Selection<DisplayComponent, TransformComponent, ShapeComponent> entities;
    PhysicsSystem const& phySys;
};