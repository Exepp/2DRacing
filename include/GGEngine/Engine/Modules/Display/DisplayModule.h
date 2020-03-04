#pragma once
#include <GGEngine/Engine/Modules/Display/DisplayLayer.h>
#include <GGEngine/Engine/Modules/Window/WindowModule.h>
#include <GGEngine/Engine/Utilities/Math/Transform.h>
#include <array>

class DisplayModule {
public:
    DisplayModule();

    void update();

    // when object's transform is in world coordinates
    void drawWorldObject(const sf::Drawable& object, const Transform& transform,
                         DisplayLayer::Level level);

    // when object's transform is in window coordinates
    void drawWindowObject(const sf::Drawable& object, DisplayLayer::Level level);

    void drawWorldPoint(Vec2f point, float size, sf::Color = sf::Color::White);

    void drawWindowPoint(Vec2f point, float size, sf::Color = sf::Color::White);

    void drawWorldLine(Vec2f start, Vec2f end, sf::Color = sf::Color::White);

    void drawWindowLine(Vec2f start, Vec2f end, sf::Color = sf::Color::White);

private:
    void displayLayers();

private:
    std::array<DisplayLayer, (uint8_t)DisplayLayer::Level::Count> layers;

    std::vector<sf::RectangleShape> points;

    sf::VertexArray lines;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Settings

template <>
struct Settings<DisplayModule> {};
