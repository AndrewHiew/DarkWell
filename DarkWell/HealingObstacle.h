#pragma once
#include "Obstacle.h"
#include "Player.h"

// HealingObstacle class derived from Obstacle
class HealingObstacle : public Obstacle {
public:
    // Default constructor
    HealingObstacle();

    // Parameterized constructor
    HealingObstacle(float x, float y, float width = 10.f, float height = 10.f);

    // Override collision effect for the healing obstacle
    void onCollision(Player& player);
};
