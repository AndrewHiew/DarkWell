#pragma once
#include <SFML/Graphics.hpp>

class Projectile {
private:
    sf::RectangleShape shape;
    float speed; // Speed of the projectile
public:
    Projectile(){}

    Projectile(float startX, float startY, float angle)
        : speed(500.0f) { // Set the speed of the projectile
        shape.setSize(sf::Vector2f(2.0f, 1.0f)); // Size of the projectile
        shape.setFillColor(sf::Color::Red); // Color of the projectile
        shape.setPosition(startX, startY);

        // Set the direction based on angle
        shape.setRotation(angle);
    }

    void update(float deltaTime) {
        // Move the projectile in the direction it's facing
        float radians = shape.getRotation() * (3.14159f / 180.0f); // Convert angle to radians
        float dx = speed * deltaTime * cos(radians);
        float dy = speed * deltaTime * sin(radians);
        shape.move(dx, dy);
    }

    sf::FloatRect getBounds() const {
        return shape.getGlobalBounds(); // Return the bounding box for collision
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape); // Draw the projectile
    }
};
