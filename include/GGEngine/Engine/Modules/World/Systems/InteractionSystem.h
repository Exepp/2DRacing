#pragma once
#include <GGEngine/Engine/Modules/World/Components/PhysicsComponent.h>
#include <GGEngine/Engine/Modules/World/Components/ShapeComponent.h>
#include <GGEngine/Engine/Modules/World/Components/TransformComponent.h>
#include <GGEngine/Engine/Modules/World/Systems/System.h>
#include <random>

class InteractionSystem : public System {
public:
    InteractionSystem();
    virtual void update(epp::EntityManager& entMgr, float dt) override;
    bool shouldDrawCollisions() const;
    bool shouldDrawCollisionGrid() const;

private:
    void drag(epp::EntityManager& entityManager);
    void spawn(epp::EntityManager& entityManager);
    void displayControll();

private:
    epp::Selection<ShapeComponent, PhysicsComponent, TransformComponent> entities;
    epp::Entity draggedEntity;
    c2Circle cursor;

    bool isDrawingCollisions = true;
    bool isDrawingCollisionGrid = true;

    std::random_device rd;
    std::uniform_real_distribution<float> dist = std::uniform_real_distribution<float>(0.f, 4.f);
};