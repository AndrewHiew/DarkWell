#include "Room.h"

Room::Room() {}

// Add an obstacle to the room
void Room::addObstacle(const Obstacle& obstacle) {
    obstacles.pushBack(obstacle);
}

// Draw all obstacles in the room
void Room::draw(sf::RenderWindow& window) {
    auto it = obstacles.getIteratorFromFront();
    while (it != it.end()) {
        it.getCurrent()->getValue().draw(window);
        ++it;
    }
}

const List<Obstacle>& Room::getObstacles() const {
    return obstacles;  // Return the list of obstacles
}