#pragma once
#include "Item.h"
#include "Projectile.h"
#include "Queue.h"
#include "Player.h"
#include <iostream>

class ItemVisitor;
class Player;

class LazerGun : public Item {

public:
    LazerGun() {  // Initialize Queue with a max size of 5
        color = sf::Color(128, 0, 128);  // Purple for Lazer Gun
    }

    std::string getName() const override { return "Lazer Gun"; }

    // Use method with parameters for shooting projectiles
    void use(float playerX, float playerY, float angle, Player* player);

    // Override the base method with no parameters
    void use() const override {
        throw std::runtime_error("Use method without parameters should not be called for LazerGun.");
    }

    virtual void accept(ItemVisitor& v) {
        v.Visit(*this);
    }
};
