#pragma once
#include "Item.h"
#include "Projectile.h"
#include "Queue.h"
#include <iostream>

class LazerGun : public Item {
private:
    Queue<Projectile> projectiles;  // Use Queue instead of List

public:
    LazerGun() : projectiles(5) {  // Initialize Queue with a max size of 5
        color = sf::Color(128, 0, 128);  // Purple for Lazer Gun
    }

    std::string getName() const override { return "Lazer Gun"; }

    // Use method with parameters for shooting projectiles
    void use(float playerX, float playerY, float angle) {
        projectiles.enqueue(Projectile(playerX, playerY, angle));
        std::cout << "Firing LazerGun at position (" << playerX << ", " << playerY << ") with angle " << angle << std::endl;
    }

    // Override the base method with no parameters
    void use() const override {
        throw std::runtime_error("Use method without parameters should not be called for LazerGun.");
    }

    // Update all projectiles
    void updateProjectiles(float deltaTime) {
        int size = projectiles.size();
        for (int i = 0; i < size; ++i) {
            projectiles.front().update(deltaTime);
            projectiles.enqueue(projectiles.dequeue());
        }
    }

    // Draw all projectiles
    void drawProjectiles(sf::RenderWindow& window) {
        int size = projectiles.size();
        for (int i = 0; i < size; ++i) {
            projectiles.front().draw(window);
            projectiles.enqueue(projectiles.dequeue());
        }
    }
};
