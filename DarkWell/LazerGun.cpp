#include "LazerGun.h"
#include "Player.h"

void LazerGun::use(float playerX, float playerY, float angle, Player* player) {
    player->getProjectiles().enqueue(Projectile(playerX, playerY, angle, player->getProjectileSpeed()));
    std::cout << "Firing LazerGun at position (" << playerX << ", " << playerY << ") with angle " << angle << std::endl;
}
