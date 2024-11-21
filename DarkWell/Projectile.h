#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Undead;  // Forward declaration of Undead class

class Projectile {
private:
    sf::RectangleShape shape;
    float speed; // Speed of the projectile

public:
    Projectile() : speed(500.0f) {}

    Projectile(float startX, float startY, float angle, float aSpeed) {
        speed = aSpeed;
        shape.setSize(sf::Vector2f(5.0f, 5.0f)); // Size of the projectile
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

    // New method to check if the projectile collides with an Undead
    bool checkCollisionWithProjectile(Projectile& projectile) {
        return getBounds().intersects(projectile.getBounds());
    }

    void setProjectileSpeed(float newSpeed) {
        speed = newSpeed;
    }

    float getProjectileSpeed() {
        return speed;
    }
};
