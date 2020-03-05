#include <GGEngine/Engine/Modules/Display/DisplayModule.h>
#include <GGEngine/Engine/Modules/World/Systems/DisplaySystem.h>
#include <GGEngine/Game.h>

void DisplaySystem::update(epp::EntityManager& entityManager, float dt)
{
    entityManager.updateSelection(entities);
    drawCollisionGrid();
    drawEntities();
}

void DisplaySystem::drawEntities()
{
    printf("%lu\n", entities.countEntities());
    auto& displayModule = Game::Modules.display;
    for (auto it = entities.begin(); it != entities.end(); ++it) {
        auto& displayC = it.getComponent<DisplayComponent>();
        auto& shapeC = it.getComponent<ShapeComponent>();
        auto& transformC = it.getComponent<TransformComponent>();

        displayC.takeShape(shapeC.getShape());
        displayModule.drawWorldObject(displayC.getShape(), transformC.getTransform(), displayC.getDisplayLevel());
    }
}

void DisplaySystem::drawCollisionGrid()
{
    static const sf::Color color(50, 50, 50);
    const float gridSize = Grid::CellSize;
    const float halfGridSize = gridSize * 0.5f;
    auto& displayModule = Game::Modules.display;

    for (auto const& chunk : phySys.getCollisionSystem().getGrid().getMap())
        if (chunk.second->ents.size() > 1) {
            Vec2i indicesSigns = signs(chunk.first);
            float left = chunk.first.x * gridSize - halfGridSize - indicesSigns.x * halfGridSize;
            float right = chunk.first.x * gridSize + halfGridSize - indicesSigns.x * halfGridSize;
            float top = chunk.first.y * gridSize - halfGridSize - indicesSigns.y * halfGridSize;
            float bottom = chunk.first.y * gridSize + halfGridSize - indicesSigns.y * halfGridSize;

            displayModule.drawWorldLine(Vec2f(left, top), Vec2f(right, top), color);
            displayModule.drawWorldLine(Vec2f(right, top), Vec2f(right, bottom), color);
            displayModule.drawWorldLine(Vec2f(right, bottom), Vec2f(left, bottom), color);
            displayModule.drawWorldLine(Vec2f(left, bottom), Vec2f(left, top), color);
        }
    displayModule.drawWorldPoint({ 0, 0 }, 0.1, sf::Color::Red);
}