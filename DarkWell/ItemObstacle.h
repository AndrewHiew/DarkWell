#pragma once
#include "Obstacle.h"
#include <string>

class ItemObstacle : public Obstacle {
private:
    std::string name;  // The name of the item

public:
    // Default constructor
    ItemObstacle();

    // Parameterized constructor
    ItemObstacle(float x, float y, const std::string& itemName);

    // Getter for the item's name
    std::string getName() const;

    // Override the draw method to render the item obstacle (10x10 purple square)
    void draw(sf::RenderWindow& window) const override;

    // Destructor (not necessary to implement unless you need specific cleanup)
    virtual ~ItemObstacle() = default;
};
