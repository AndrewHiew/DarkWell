#pragma once
#include <SFML/Graphics.hpp>

class Obstacle {
protected:
    sf::RectangleShape shape;  // The shape representing the obstacle

public:
    // Default constructor (needed by the linked list)
    Obstacle();

    // Parameterized constructor
    Obstacle(float x, float y, float width, float height);

    // Draw the obstacle using the provided window
    virtual void draw(sf::RenderWindow& window) const;

    // Get the obstacle's shape (for potential collision detection)
    const sf::RectangleShape& getShape() const;
    sf::FloatRect getBounds() const;

    // Set position for the obstacle
    void setPosition(float x, float y);

    // Virtual destructor for proper cleanup of derived classes
    virtual ~Obstacle() = default;
};
