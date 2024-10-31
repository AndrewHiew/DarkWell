#include <SFML/Graphics.hpp>
#include "GameEngine.h"
#include "Room.h"
#include "Player.h"
#include "MovingObstacle.h"
#include "GameMap.h"
#include <iostream>

GameEngine::GameEngine() : gameMap(new GameMap()) {}  // Initialize GameMap

GameEngine::~GameEngine() { cleanUp(); }

int GameEngine::StartGame() {
    sf::RenderWindow window(sf::VideoMode(640, 360), "DarkWell");
    window.setFramerateLimit(60);

    Player player(100);
    player.setPosition(50, 280);

    // Initialize rooms and set up their connections within the GameMap
    Room* room1 = initializeRoom1();
    Room* room2 = initializeRoom2();
    Room* room3 = initializeRoom3();

    gameMap->setRootRoom(room1);             // Set Room 1 as the root room
    gameMap->addRoom(room1, room2, "left"); // Link Room 1 to Room 2 on the right
    gameMap->addRoom(room1, room3, "up");    // Link Room 1 to Room 3 above
    gameMap->addRoom(room2, room1, "left");  // Link Room 2 back to Room 1 on the left
    gameMap->addRoom(room3, room1, "down");  // Link Room 3 back to Room 1 below

    gameMap->setCurrentRoom(room1);          // Start the player in Room 1

    sf::Clock clock;

    while (window.isOpen()) {
        handleEvents(window);
        float deltaTime = clock.restart().asSeconds();

        player.handleInput();
        player.update(deltaTime, *gameMap->getCurrentRoom());
        updateObstacles(gameMap->getCurrentRoom(), deltaTime);

        // Check for room transitions
        handleRoomTransitions(player);

        window.clear(sf::Color::White);
        gameMap->getCurrentRoom()->draw(window);
        player.draw(window);
        window.display();
    }

    return 0;
}

Room* GameEngine::initializeRoom1() {
    Room* room1 = new Room("Room 1");
    room1->addObstacle(new NormalObstacle(0, 0, 300, 10));  // Ceiling obstacle
    room1->addObstacle(new NormalObstacle(340, 0, 300, 10));  // Ceiling obstacle

    room1->addObstacle(new NormalObstacle(0, 350, 300, 10));  // Floor obstacle
    room1->addObstacle(new NormalObstacle(340, 350, 300, 10));  // Floor obstacle

    room1->addObstacle(new NormalObstacle(300, 250, 100, 20));
    room1->addObstacle(new NormalObstacle(100, 200, 100, 20));
    room1->addObstacle(new MovingObstacle(300, 150, 80, 20, 100.0f, 50.0f, 170.0f)); // Moving obstacle
    room1->addObstacle(new NormalObstacle(500, 100, 60, 20));
    //room1->addObstacle(new NormalObstacle(0, 0, 10, 360));
    return room1;
}

Room* GameEngine::initializeRoom2() {
    Room* room2 = new Room("Room 2");
    room2->addObstacle(new NormalObstacle(0, 0, 300, 10));  // Ceiling obstacle
    room2->addObstacle(new NormalObstacle(340, 0, 300, 10));  // Ceiling obstacle

    room2->addObstacle(new NormalObstacle(0, 350, 300, 10));  // Floor obstacle
    room2->addObstacle(new NormalObstacle(340, 350, 300, 10));  // Floor obstacle

    room2->addObstacle(new NormalObstacle(300, 250, 100, 20));
    room2->addObstacle(new NormalObstacle(100, 200, 100, 20));
    room2->addObstacle(new MovingObstacle(300, 150, 80, 20, 100.0f, 50.0f, 170.0f)); // Moving obstacle
    room2->addObstacle(new NormalObstacle(500, 100, 60, 20));
    //room2->addObstacle(new NormalObstacle(0, 0, 10, 360));
    return room2;
}

Room* GameEngine::initializeRoom3() {
    Room* room3 = new Room("Room 3");
    room3->addObstacle(new NormalObstacle(0, 0, 300, 10));  // Ceiling obstacle
    room3->addObstacle(new NormalObstacle(340, 0, 300, 10));  // Ceiling obstacle

    room3->addObstacle(new NormalObstacle(0, 350, 300, 10));  // Floor obstacle
    room3->addObstacle(new NormalObstacle(340, 350, 300, 10));  // Floor obstacle

    room3->addObstacle(new NormalObstacle(300, 250, 100, 20));
    room3->addObstacle(new NormalObstacle(100, 200, 100, 20));
    room3->addObstacle(new MovingObstacle(300, 150, 80, 20, 100.0f, 50.0f, 170.0f)); // Moving obstacle
    room3->addObstacle(new NormalObstacle(500, 100, 60, 20));
    //room3->addObstacle(new NormalObstacle(0, 0, 10, 360));
    return room3;
}

void GameEngine::handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

void GameEngine::updateObstacles(Room* currentRoom, float deltaTime) {
    const auto& obstacles = currentRoom->getObstacles();

    // Iterate over all obstacles to find any MovingObstacle
    auto it = obstacles.getIteratorFromFront();
    while (it != it.end()) {
        Obstacle* obstacle = it.getCurrent()->getValue();

        MovingObstacle* movingObstacle = dynamic_cast<MovingObstacle*>(obstacle);
        if (movingObstacle) {
            movingObstacle->update(deltaTime);  // Update the moving obstacle
        }
        ++it;
    }
}

void GameEngine::handleRoomTransitions(Player& player) {
    sf::FloatRect playerBounds = player.getBounds();

    if (playerBounds.left + playerBounds.width > 640) {  // Right transition
        gameMap->handleRoomTransition("right");
        player.setPosition(0, player.getPosition().y);
    }
    else if (playerBounds.left < 0) {  // Left transition
        gameMap->handleRoomTransition("left");
        player.setPosition(640 - playerBounds.width, player.getPosition().y);
    }
    else if (playerBounds.top < 0) {  // Upward transition
        gameMap->handleRoomTransition("up");
        player.setPosition(player.getPosition().x, 280);
    }
    else if (playerBounds.top + playerBounds.height > 360) {  // Downward transition
        gameMap->handleRoomTransition("down");
        player.setPosition(player.getPosition().x, 0);
    }
}

void GameEngine::cleanUp() {
    delete gameMap;  // Deallocate map and rooms
}
