#include <SFML/Graphics.hpp>
#include "GameEngine.h"
#include "Room.h"
#include "Player.h"
#include "MovingObstacle.h"
#include <iostream>
#include "List.h"
#include "LazerGun.h"
#include "Shovel.h"
#include "ItemObstacle.h"

// Default constructor
GameEngine::GameEngine() : gamePaused(false) {}  // Initialize gamePaused flag

// Destructor
GameEngine::~GameEngine() { cleanUp(); }

// Main method to start the game
int GameEngine::StartGame() {
    sf::RenderWindow window(sf::VideoMode(640, 360), "DarkWell");
    window.setFramerateLimit(60);

    Player player(60);
    player.setPosition(50, 318);  // Initial position in Room 1

    // Add items to player's inventory`
    //player.addItemToInventory(new LazerGun());
    //player.addItemToInventory(new Shovel());

    // Load and start playing background music
    if (!backgroundMusic.openFromFile("lacrimosa.mp3")) {
        std::cerr << "Failed to load lacrimosa.mp3!" << std::endl;
        return -1;  // Exit if music fails to load
    }

    backgroundMusic.setLoop(true);    // Enable looping
    backgroundMusic.setVolume(30);    // Set volume (adjust as needed)
    backgroundMusic.play();

    // Initialize rooms and their obstacles
    rooms.pushBack(initializeRoom1());
    rooms.pushBack(initializeRoom2());
    rooms.pushBack(initializeRoom3());
    rooms.pushBack(initializeRoom4());
    Room* currentRoom = rooms[0];  // Start in Room 1

    sf::Clock clock;  // For deltaTime calculations

    bool isRespawning = false;

    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Check for respawn if player is dead
            if (player.getPlayerDead() && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                currentRoom = rooms[0];       // Set current room to Room 1
                player.respawn();
                player.setCurrentHP(60);     // Reset health
                player.setPlayerDead(false);
                isRespawning = true;
                gamePaused = false;  // Unpause the game when respawning
                respawnNPCdead();

                // Additional Debug Info
                std::cout << "Player position after respawn: ("
                    << player.getPosition().x << ", "
                    << player.getPosition().y << ")" << std::endl;
            }
        }

        if (player.getPlayerDead()) {
            // Pause the game and show respawn dialog
            gamePaused = true;  // Set game as paused

            sf::Text deathText;
            sf::Font font;
            if (!font.loadFromFile("arial.ttf")) {  // Load a font (make sure the file exists)
                std::cerr << "Failed to load font!" << std::endl;
            }
            deathText.setFont(font);
            deathText.setString("You Died! Press 'R' to Respawn");
            deathText.setCharacterSize(24);
            deathText.setFillColor(sf::Color::Red);
            deathText.setPosition(150, 180);

            window.clear(sf::Color::Black);
            window.draw(deathText);
            window.display();
            continue;  // Skip the rest of the update and render when player is dead
        }

        if (gamePaused) continue;  // Skip game updates if paused

        float deltaTime = clock.restart().asSeconds();

        player.handleInput();
        updateObstacles(currentRoom, deltaTime);
        handleRoomTransitions(player, currentRoom, rooms, isRespawning);

        isRespawning = false;

        // Check for collision with KillObstacle
        if (currentRoom->checkKillCollision(player.getBounds())) {
            player.setPlayerDead(true);  // Player died
            gamePaused = true;  // Pause the game
            continue;  // Skip the rest of the frame
        }

        window.clear(sf::Color::Black);
        spawnNPC(currentRoom);
        currentRoom->draw(window);
        currentRoom->update(deltaTime, player, window);

        player.draw(window);

        // Update logic
        player.update(deltaTime, *currentRoom);
        currentRoom->updateProjectile(deltaTime, player, window);
        

        window.display();
    }

    return 0;
}

// Method to spawn NPCs
void GameEngine::spawnNPC(Room* currentRoom) {
    // Check if NPCs are already spawned
    if (currentRoom->getName() == "Room 2" && !currentRoom->areNPCsSpawned()) {
        Undead* undead1 = new Undead(100, 500, 200);
        currentRoom->getCharacters().pushBack(undead1);

        // Set the flag to indicate NPCs have been spawned
        currentRoom->setNPCsSpawned(true);
    }
}

// Method to respawn NPCs
void GameEngine::respawnNPC() {
    typename List<Room*>::Iterator it = rooms.getIteratorFromFront();
    while (it != it.end()) {
        Room* room = it.getCurrent()->getValue();
        room->resetNPC();  // Reset NPCs
        ++it;
    }
}

// Method to respawn the NPCs when player dies.
void GameEngine::respawnNPCdead() {
    typename List<Room*>::Iterator it = rooms.getIteratorFromFront();
    while (it != it.end()) {
        Room* room = it.getCurrent()->getValue();
        room->resetNPCdead();
        room->setNPCsSpawned(false);  // Reset spawn flag
        ++it;
    }
}

// Method to draw the Inventory Overlay
void GameEngine::drawInventoryOverlay(sf::RenderWindow& window, Player& player, sf::RectangleShape& lazerGunShape, sf::RectangleShape& shovelShape) {
    // Draw the Lazer Gun shape
    window.draw(lazerGunShape);

    // Draw the Shovel shape
    window.draw(shovelShape);

    // Highlight the selected item
    sf::RectangleShape* selectedShape = (player.getSelectedItemIndex() == 0) ? &lazerGunShape : &shovelShape;
    selectedShape->setOutlineThickness(3);
    selectedShape->setOutlineColor(sf::Color::Yellow);  // Highlight color for selected item

    // Draw again to show the highlight
    window.draw(*selectedShape);

    // Remove highlight for next frame
    selectedShape->setOutlineThickness(0);
}


// Methods to initialize Rooms 
Room* GameEngine::initializeRoom1() {
    Room* room1 = new Room("Room 1");

    room1->addObstacle(new ItemObstacle(150, 180, new LazerGun())); // The LazerGun Object
    room1->addObstacle(new ItemObstacle(200, 125, new Shovel()));

    room1->addObstacle(new NormalObstacle(0, 350, 300, 10));  // Floor obstacle
    room1->addObstacle(new NormalObstacle(340, 350, 300, 10));  // Floor obstacle

    room1->addObstacle(new NormalObstacle(0, 0, 300, 10));  // Celling obstacle
    room1->addObstacle(new NormalObstacle(340, 0, 300, 10));  // Celling obstacle

    room1->addObstacle(new NormalObstacle(0, 0, 10, 360));  // Left Wall
    room1->addObstacle(new NormalObstacle(630, 0, 10, 300));  // Right Wall

    room1->addObstacle(new NormalObstacle(300, 250, 100, 20)); // Left Platform
    room1->addObstacle(new NormalObstacle(100, 200, 100, 20)); // Middle Platform
    room1->addObstacle(new NormalObstacle(500, 100, 60, 20)); // Top Right Platform

    room1->addObstacle(new MovingObstacle(300, 150, 80, 20, 100.0f, 50.0f, 150.0f)); // Add moving obstacle
    room1->addObstacle(new MovingObstacle(0, 150, 80, 20, 100.0f, 50.0f, 150.0f)); // Add moving obstacle

    room1->addObstacle(new KillObstacle(200, 300, 20, 20)); // Add KillObstacle
    room1->addObstacle(new KillObstacle(400, 340, 20, 20)); // Add KillObstacle
    room1->addObstacle(new KillObstacle(0, 360, 640, 20)); // Add KillObstacle

    return room1;
}

Room* GameEngine::initializeRoom2() {
    Room* room2 = new Room("Room 2");
    room2->addObstacle(new NormalObstacle(0, 350, 300, 10));  // Floor obstacle
    room2->addObstacle(new NormalObstacle(340, 350, 300, 10));  // Floor obstacle

    room2->addObstacle(new NormalObstacle(200, 200, 80, 20));
    room2->addObstacle(new NormalObstacle(400, 150, 100, 20));
    return room2;
}

Room* GameEngine::initializeRoom3() {
    Room* room3 = new Room("Room 3");
    room3->addObstacle(new NormalObstacle(0, 350, 300, 10));  // Floor obstacle
    room3->addObstacle(new NormalObstacle(340, 350, 300, 10));  // Floor obstacle
    room3->addObstacle(new NormalObstacle(300, 200, 80, 20)); // Wall on the left of the gap
    room3->addObstacle(new NormalObstacle(380, 200, 80, 20)); // Wall on the right of the gap
    return room3;
}

Room* GameEngine::initializeRoom4() {
    Room* room4 = new Room("Room 4");
    room4->addObstacle(new NormalObstacle(0, 350, 300, 10));  // Floor obstacle
    room4->addObstacle(new NormalObstacle(340, 350, 300, 10));  // Floor obstacle

    room4->addObstacle(new NormalObstacle(200, 200, 80, 20));
    room4->addObstacle(new NormalObstacle(400, 150, 100, 20));
    return room4;
}

Room* GameEngine::initializeRoom5() {
    Room* room5 = new Room("Room 5");
    return room5;
}

void GameEngine::handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

// Method to update moving obstacle
void GameEngine::updateObstacles(Room* currentRoom, float deltaTime) {
    // Use List's custom iterator to iterate over all obstacles
    typename List<Obstacle*>::Iterator it = currentRoom->getObstacles().getIteratorFromFront();
    while (it != it.end()) {
        Obstacle* obstacle = it.getCurrent()->getValue();
        MovingObstacle* movingObstacle = dynamic_cast<MovingObstacle*>(obstacle);
        if (movingObstacle) {
            movingObstacle->update(deltaTime);  // Update moving obstacle
        }
        ++it;  // Move to the next obstacle
    }
}

// Method to handle room transitions
void GameEngine::handleRoomTransitions(Player& player, Room*& currentRoom, List<Room*>& rooms, bool& isRespawning) {
    if (isRespawning) return;

    sf::FloatRect playerBounds = player.getBounds();

    // Check transitions to different rooms
    if (playerBounds.left + playerBounds.width > 640 && currentRoom == rooms[0]) {  // Room 1 to Room 2
        player.setPosition(0, player.getPosition().y);  // Place player at the left edge of Room 2
        currentRoom = rooms[1];  // Transition to Room 2
        respawnNPC();
    }
    else if (playerBounds.left < 0 && currentRoom == rooms[1]) {  // Room 2 to Room 1
        player.setPosition(640 - playerBounds.width, player.getPosition().y);  // Place player at right edge of Room 1
        currentRoom = rooms[0];  // Transition to Room 1
        respawnNPC();
    }
    else if (playerBounds.top < 0 && currentRoom == rooms[0]) {  // Room 1 to Room 3
        player.setPosition(player.getPosition().x, 280);  // Reset position for Room 3
        currentRoom = rooms[2];  // Transition to Room 3
        respawnNPC();
    }
    else if (playerBounds.top + playerBounds.height > 360 && currentRoom == rooms[2]) {  // Room 3 to Room 1
        player.setPosition(player.getPosition().x, 0);  // Reset position for Room 1
        currentRoom = rooms[0];  // Transition to Room 1
        respawnNPC();
    }
    else if (playerBounds.left + playerBounds.width > 640 && currentRoom == rooms[1]) {  // Room 2 to Room 4
        player.setPosition(0, player.getPosition().y);  // Place player at the left edge of Room 4
        currentRoom = rooms[3];  // Transition to Room 4
        respawnNPC();
    }
    else if (playerBounds.left < 0 && currentRoom == rooms[3]) {  // Room 4 to Room 2
        player.setPosition(640 - playerBounds.width, player.getPosition().y);  // Place player at right edge of Room 2
        currentRoom = rooms[1];  // Transition to Room 2
        respawnNPC();
    }
}

// Method to delete rooms
void GameEngine::cleanUp() {
    typename List<Room*>::Iterator it = rooms.getIteratorFromFront();
    while (it != it.end()) {
        Room* room = it.getCurrent()->getValue();
        ++it;
        std::cout << "Deleting: " << room->getName() << std::endl;
        delete room;  // Delete each room
    }
}

