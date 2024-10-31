#include <SFML/Graphics.hpp>
#include "GameEngine.h"
#include "Room.h"
#include "Player.h"
#include "RoomNode.h"

GameEngine::GameEngine() {}

GameEngine::~GameEngine() {}

int GameEngine::StartGame() {
    sf::RenderWindow window(sf::VideoMode(640, 360), "DarkWell - Room Transition Test");
    window.setFramerateLimit(60);

    // Initialize player
    Player player(100);  // Create a player with 100 HP
    player.setPosition(50, 280);  // Initial position in Room 1

    // Create two rooms
    Room* room1 = new Room();
    Room* room2 = new Room();

    // Create room nodes and link them
    RoomNode* roomNode1 = new RoomNode(room1);
    RoomNode* roomNode2 = new RoomNode(room2);
    roomNode1->right = roomNode2;
    roomNode2->left = roomNode1;

    // Set up obstacles in Room 1
    room1->addObstacle(Obstacle(0, 320, 640, 40));  // Floor obstacle
    room1->addObstacle(Obstacle(300, 250, 100, 20));
    room1->addObstacle(Obstacle(100, 200, 100, 20));
    room1->addObstacle(Obstacle(300, 150, 80, 20));
    room1->addObstacle(Obstacle(500, 100, 60, 20));

    // Set up obstacles in Room 2
    room2->addObstacle(Obstacle(0, 320, 640, 40));
    room2->addObstacle(Obstacle(200, 200, 80, 20));
    room2->addObstacle(Obstacle(400, 150, 100, 20));

    Room* currentRoom = room1;  // Start in Room 1
    sf::Clock clock;  // For deltaTime calculations

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();  // Calculate deltaTime

        player.handleInput();  // Handle keyboard input
        player.update(deltaTime, *currentRoom);  // Update player with room collision

        // Check if player has moved past the room boundaries for room transition
        sf::FloatRect playerBounds = player.getBounds();
        if (playerBounds.left + playerBounds.width > 640 && currentRoom == room1) {  // Moving right to Room 2
            player.setPosition(0, player.getPosition().y);  // Place player at the left edge of Room 2
            currentRoom = room2;
        }
        else if (playerBounds.left < 0 && currentRoom == room2) {  // Moving left to Room 1
            player.setPosition(640 - playerBounds.width, player.getPosition().y);  // Place player at right edge of Room 1
            currentRoom = room1;
        }

        window.clear(sf::Color::White);  // Clear the window
        currentRoom->draw(window);  // Draw the current room and its obstacles
        player.draw(window);  // Draw the player
        window.display();  // Display everything
    }

    // Clean up room nodes
    delete roomNode1;
    delete roomNode2;

    return 0;
}
