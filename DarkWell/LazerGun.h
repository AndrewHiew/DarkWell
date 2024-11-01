#pragma once
#include "Item.h"
#include "Projectile.h"
#include "List.h"
#include <iostream>


class LazerGun : public Item {
private:
    List<Projectile> projectiles;

public:
    LazerGun() {
        color = sf::Color(128, 0, 128);  // Purple for Lazer Gun
    }

    std::string getName() const override { return "Lazer Gun"; }

    // Keep the previous method
    void use(float playerX, float playerY, float angle) {
        // Implementation for shooting projectile
        projectiles.pushBack(Projectile(playerX, playerY, angle));
        std::cout << "Firing LazerGun at position (" << playerX << ", " << playerY << ") with angle " << angle << std::endl;
    }

    // Override the base method with no parameters
    void use() const override {
        // Optionally throw an error or handle as needed, or provide default behavior
        throw std::runtime_error("Use method without parameters should not be called for LazerGun.");
    }

    void updateProjectiles(float deltaTime) {
        // Update all projectiles
        // std::cout << "Updating projectiles. Total: " << projectiles.size() << std::endl;
        for (int i = 0; i < projectiles.size(); ++i) {
            projectiles[i].update(deltaTime);
        }
    }

    void drawProjectiles(sf::RenderWindow& window) {
        // Draw all projectiles
        // std::cout << "Drawing projectiles. Total: " << projectiles.size() << std::endl;
        for (int i = 0; i < projectiles.size(); ++i) {
            projectiles[i].draw(window);
        }
    }
};
