#pragma once

#include "Obstacle.h"
#include "Item.h"
#include <SFML/Graphics.hpp>
#include <string>

class ItemObstacle : public Obstacle {
private:
    Item* item;

public:
    // Default constructor
    ItemObstacle();

    // Parameterized constructor
    ItemObstacle(float x, float y, Item* aItem);

    // Getter for the item's name
    Item* getItem();

    // Override the draw method to render the item obstacle (10x10 purple square)
    void draw(sf::RenderWindow& window) const override;

    // Destructor (not necessary to implement unless you need specific cleanup)
    virtual ~ItemObstacle() = default;
};
