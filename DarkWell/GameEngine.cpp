#include <SFML/Graphics.hpp>
#include "GameEngine.h"
#include "Room.h"
#include "Player.h"
#include "RoomNode.h"
#include "MovingObstacle.h"
#include <iostream>
#include "List.h"  // Include your List template

GameEngine::GameEngine() {}

GameEngine::~GameEngine() { cleanUp(); }

int GameEngine::StartGame() {
    sf::RenderWindow window(sf::VideoMode(640, 360), "DarkWell - Room Transition Test");
    window.setFramerateLimit(60);

    Player player(100);  // Create a player with 100 HP
    player.setPosition(50, 280);  // Initial position in Room 1

    // Initialize rooms and their obstacles using the List
    rooms.pushBack(initializeRoom1());
    rooms.pushBack(initializeRoom2());
    rooms.pushBack(initializeRoom3());
    Room* currentRoom = rooms[0];  // Start in Room 1
    sf::Clock clock;  // For deltaTime calculations

    while (window.isOpen()) {
        handleEvents(window);

        float deltaTime = clock.restart().asSeconds();  // Calculate deltaTime

        player.handleInput();  // Handle keyboard input
        player.update(deltaTime, *currentRoom);  // Update player with room collision

        updateObstacles(currentRoom, deltaTime);

        // Check for room transitions
        handleRoomTransitions(player, currentRoom, rooms);  // Pass List of rooms

        window.clear(sf::Color::White);  // Clear the window
        currentRoom->draw(window);  // Draw the current room and its obstacles
        player.draw(window);  // Draw the player
        window.display();  // Display everything
    }

    return 0;
}

Room* GameEngine::initializeRoom1() {
    Room* room1 = new Room("Room 1");
    room1->addObstacle(new NormalObstacle(0, 320, 640, 40));  // Floor obstacle
    room1->addObstacle(new NormalObstacle(300, 250, 100, 20));
    room1->addObstacle(new NormalObstacle(100, 200, 100, 20));
    room1->addObstacle(new MovingObstacle(300, 150, 80, 20, 100.0f, 100.0f, 150.0f)); // Add moving obstacle
    room1->addObstacle(new NormalObstacle(500, 100, 60, 20));
    return room1;
}

Room* GameEngine::initializeRoom2() {
    Room* room2 = new Room("Room 2");
    room2->addObstacle(new NormalObstacle(0, 320, 640, 40));
    room2->addObstacle(new NormalObstacle(200, 200, 80, 20));
    room2->addObstacle(new NormalObstacle(400, 150, 100, 20));
    return room2;
}

Room* GameEngine::initializeRoom3() {
    Room* room3 = new Room("Room 3");
    room3->addObstacle(new NormalObstacle(0, 320, 300, 40));  // Floor obstacle
    room3->addObstacle(new NormalObstacle(340, 320, 300, 40));  // Floor obstacle
    room3->addObstacle(new NormalObstacle(300, 200, 80, 20)); // Wall on the left of the gap
    room3->addObstacle(new NormalObstacle(380, 200, 80, 20)); // Wall on the right of the gap
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
    const auto& obstacles = currentRoom->getObstacles(); // Get a reference to obstacles
    std::cout << "Number of obstacles: " << obstacles.size() << std::endl;

    if (obstacles.size() > 3) { // Ensure that there are at least 4 obstacles
        MovingObstacle* movingObstacle = dynamic_cast<MovingObstacle*>(obstacles[3]); // Assuming it is the fourth obstacle
        if (movingObstacle) {
            movingObstacle->update(deltaTime);
        }
    }
    else {
        std::cerr << "Error: Not enough obstacles in the room. Expected at least 4." << std::endl;
    }
}

void GameEngine::handleRoomTransitions(Player& player, Room*& currentRoom, List<Room*>& rooms) {
    sf::FloatRect playerBounds = player.getBounds();

    // Check transitions to different rooms
    if (playerBounds.left + playerBounds.width > 640 && currentRoom == rooms[0]) {  // Room 1 to Room 2
        player.setPosition(0, player.getPosition().y);  // Place player at the left edge of Room 2
        currentRoom = rooms[1];  // Transition to Room 2
    }
    else if (playerBounds.left < 0 && currentRoom == rooms[1]) {  // Room 2 to Room 1
        player.setPosition(640 - playerBounds.width, player.getPosition().y);  // Place player at right edge of Room 1
        currentRoom = rooms[0];  // Transition to Room 1
    }
    else if (playerBounds.top < 0 && currentRoom == rooms[0]) {  // Room 1 to Room 3
        player.setPosition(player.getPosition().x, 280);  // Reset position for Room 3
        currentRoom = rooms[2];  // Transition to Room 3
    }
    else if (playerBounds.top + playerBounds.height > 360 && currentRoom == rooms[2]) {  // Room 3 to Room 1
        player.setPosition(player.getPosition().x, 0);  // Reset position for Room 1
        currentRoom = rooms[0];  // Transition to Room 1
    }
}

void GameEngine::cleanUp() {
    // Use 'typename' to indicate that 'Iterator' is a type
    typename List<Room*>::Iterator it = rooms.getIteratorFromFront();
    while (it != it.end()) {
        Room* room = it.getCurrent()->getValue();
        ++it;
        std::cout << "Deleting: " << room->getName() << std::endl;
        delete room;  // Delete each room
    }
}

