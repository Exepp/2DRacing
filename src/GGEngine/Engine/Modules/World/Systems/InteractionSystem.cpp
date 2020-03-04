#include <GGEngine/Engine/Modules/World/Components/DisplayComponent.h>
#include <GGEngine/Engine/Modules/World/Systems/InteractionSystem.h>
#include <GGEngine/Game.h>

InteractionSystem::InteractionSystem()
{
    cursor.r = 0;
}

void InteractionSystem::update(epp::EntityManager& entityManager, float dt)
{
    entityManager.updateSelection(entities);
    displayControll();
    drag(entityManager);
    spawn(entityManager);
}

void InteractionSystem::drag(epp::EntityManager& entMgr)
{
    const ControllerModule& controllerModule = Game::Modules.controller;

    if (controllerModule.getActionKeyState(ActionKey::Options) == KeyState::Pressed)
        Game::Modules.window.getWin().close();

    if (draggedEntity == epp::Entity() && controllerModule.getActionKeyState(ActionKey::LMB) == KeyState::Pressed) {
        cursor.p = asVec<c2v>(controllerModule.getCursorWorldPosition());
        for (auto it = entities.begin(); it != entities.end(); ++it)
            if (it.getComponent<ShapeComponent>().getShape().isColliding(&cursor, C2_TYPE::C2_CIRCLE)) {
                draggedEntity = *it;
                break;
            }
    }
    else if (controllerModule.getActionKeyState(ActionKey::LMB) == KeyState::Released)
        draggedEntity = epp::Entity();

    if (entMgr.isValid(draggedEntity)) {
        auto& tc = entMgr.componentOf<TransformComponent>(draggedEntity);
        auto& pc = entMgr.componentOf<PhysicsComponent>(draggedEntity);
        auto diff = controllerModule.getCursorWorldPosition() - tc.getPosition();
        float diffLen = diff.length();
        diffLen = std::clamp(diffLen, 0.f, 200.f);
        // printf("%f \n", diffLen);
        pc.velocity = diff.normalize() * diffLen * diffLen;
        pc.velocity *= 0.99f;
        pc.velocity = pc.velocity.normalized() * std::clamp(pc.velocity.length(), 0.f, 1000.f);
    }
}

void InteractionSystem::spawn(epp::EntityManager& entityManager)
{
    static epp::Archetype arch(epp::IdOf<TransformComponent, ShapeComponent, PhysicsComponent, DisplayComponent>());
    const ControllerModule& controllerModule = Game::Modules.controller;

    if (controllerModule.getActionKeyState(ActionKey::RMB) != KeyState::Released) {
        for (int i = 0; i < 10; i++) {
            auto spawned = entityManager.spawn(arch, [&](epp::EntitySpawner::Creator&& creator) {
                auto& shC = creator.constructed<ShapeComponent>();
                auto& phC = creator.constructed<PhysicsComponent>();
                auto& trC = creator.constructed<TransformComponent>();
                if (dist(rd) <= 2.f) {
                    float a = 0.5f + dist(rd);
                    float b = 0.5f + dist(rd);
                    shC.takeShape<PolygonShape>().setAsBox(Vec2f(a, b));
                    phC.setMass(a * b);
                    phC.moi = 1.f / 12.f * a * b * (a * a + b * b);
                    phC.moiInv = 1.f / phC.moi;
                    trC.setPosition(controllerModule.getCursorWorldPosition());
                }
                else {

                    float r = 0.5f + dist(rd);
                    shC.takeShape<CircleShape>().setRadius(r);
                    phC.setMass(PI_F * r * r);
                    phC.moi = 1.f / 2.f * PI_F * r * r * r * r;
                    phC.moiInv = 1.f / phC.moi;
                    trC.setPosition(controllerModule.getCursorWorldPosition());
                }
            });
        }
    }
}

void InteractionSystem::displayControll()
{
    const ControllerModule& controllerModule = Game::Modules.controller;

    if (controllerModule.getActionKeyState(ActionKey::Num1) == KeyState::Pressed)
        isDrawingCollisions = !isDrawingCollisions;
    if (controllerModule.getActionKeyState(ActionKey::Num2) == KeyState::Pressed)
        isDrawingCollisionGrid = !isDrawingCollisionGrid;
}

bool InteractionSystem::shouldDrawCollisions() const
{
    return isDrawingCollisions;
}

bool InteractionSystem::shouldDrawCollisionGrid() const
{
    return isDrawingCollisionGrid;
}