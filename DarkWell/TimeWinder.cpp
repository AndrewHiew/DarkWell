#include "TimeWinder.h"
#include "Player.h"

void TimeWinder::use(Player* player) {
    // Ensure the stack has at least 5 elements
    if (player->getPlayerPositionHistory()->size() < 5) {
        std::cout << "cannot use TimeWinder yet" << std::endl;
    }
    else {
        // Get the 5th position (index 4)
        // sf::Vector2f position = player->getPlayerPositionHistory()->top(); // Get the last element

        // Pop the topmost (most recent) position and get the 5th one by accessing previous ones
        for (int i = 0; i < 4; ++i) {  // Pop off the last 4 positions
            sf::Vector2f position = player->getPlayerPositionHistory()->pop();
            player->setPosition(position.x, position.y);
        }
    }
}