#include <GGEngine/Engine/Modules/World/Systems/CollisionSystem/Grid.h>


void Grid::insert(epp::Entity ent, Vec2f const& pos, AABBf const& aabb)
{
    AABBi mapBounds = CalcMapBounds(pos, aabb);
    if (mapBounds.min == mapBounds.max) {
        Cell_t*& ptr = cellsMap[mapBounds.min];
        if (!ptr)
            ptr = &cellsDeq.emplace_back();
        ptr->ents.push_back(ent);
        ptr->bounds.push_back(mapBounds);
    }
    else {
        bigEnts.ents.push_back(ent);
        bigEnts.bounds.push_back(mapBounds);
    }
    // AABBi mapBounds = CalcMapBounds(pos, aabb);
    // for (ptrdiff_t x = mapBounds.min.x; x <= mapBounds.max.x; ++x)
    //     for (ptrdiff_t y = mapBounds.min.y; x != 0 && y <= mapBounds.max.y; ++y)
    //         if (y != 0) {
    //             Cell_t*& ptr = cellsMap[Vec2i(x, y)];
    //             if (!ptr)
    //                 ptr = &cellsDeq.emplace_back();
    //             ptr->ents.push_back(ent);
    //             ptr->bounds.push_back(mapBounds);
    //         }
}

void Grid::swapWithBigEnts()
{
    cellsMap.clear();
    cellsDeq.clear();
    for (std::size_t i = 0; i < bigEnts.ents.size(); ++i)
        for (ptrdiff_t x = bigEnts.bounds[i].min.x; x <= bigEnts.bounds[i].max.x; ++x)
            for (ptrdiff_t y = bigEnts.bounds[i].min.y; x != 0 && y <= bigEnts.bounds[i].max.y; ++y)
                if (y != 0) {
                    Cell_t*& ptr = cellsMap[Vec2i(x, y)];
                    if (!ptr)
                        ptr = &cellsDeq.emplace_back();
                    ptr->ents.push_back(bigEnts.ents[i]);
                    ptr->bounds.push_back(bigEnts.bounds[i]);
                }
}

void Grid::clear()
{
    cellsMap.clear();
    cellsDeq.clear();
    bigEnts.ents.clear();
    bigEnts.bounds.clear();
}