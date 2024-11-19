#include "NormalObstacle.h"
#include <SFML/Graphics.hpp>
#include <iostream>

// Default constructor
NormalObstacle::NormalObstacle() : Obstacle(), brickTexture(nullptr) {}

// Parameterized constructor
NormalObstacle::NormalObstacle(float x, float y, float width, float height)
    : Obstacle(x, y, width, height), brickTexture(new sf::Texture()) {
    shape.setSize(sf::Vector2f(width, height)); // Set the size of the shape
    shape.setPosition(x, y); // Set the position of the shape

    // Load the texture
    if (!brickTexture->loadFromFile("brick.png")) {
        std::cerr << "Failed to load texture: brick.png" << std::endl;
    }

    // Apply the brick texture
    shape.setTexture(brickTexture);

    // Set the texture to repeat
    brickTexture->setRepeated(true);

    // Set the texture rect to tile for 10x10 pixels
    shape.setTextureRect(sf::IntRect(0, 0, static_cast<int>(width), static_cast<int>(height)));

    // Set the fill color to white to avoid tinting from the shape's color
    shape.setFillColor(sf::Color::White);
}

// Constructor Overload
NormalObstacle::NormalObstacle(float x, float y, float width, float height, sf::Color color)
    : Obstacle(x, y, width, height), brickTexture(new sf::Texture()) {
    shape.setSize(sf::Vector2f(width, height)); // Set the size of the shape
    shape.setPosition(x, y); // Set the position of the shape

    // Load the texture
    if (!brickTexture->loadFromFile("brick.png")) {
        std::cerr << "Failed to load texture: brick.png" << std::endl;
    }

    // Apply the brick texture
    shape.setTexture(brickTexture);

    // Set the texture to repeat
    brickTexture->setRepeated(true);

    // Set the texture rect to tile the entire obstacle's area
    shape.setTextureRect(sf::IntRect(0, 0, static_cast<int>(width), static_cast<int>(height)));

    // Set the fill color to the color passed into the constructor
    shape.setFillColor(color);
}



// Destructor to clean up dynamic texture
NormalObstacle::~NormalObstacle() {
    delete brickTexture; // Ensure memory is freed
}

// Draw method implementation
void NormalObstacle::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}
