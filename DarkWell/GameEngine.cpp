#pragma once
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
#include "Juggernaut.h"
#include "HealingObstacle.h"

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
    player.addItemToInventory(new LazerGun());
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
    rooms.pushBack(initializeRoom5());
    rooms.pushBack(initializeRoom6());
    rooms.pushBack(initializeRoom7());
    rooms.pushBack(initializeRoom8());
    Room* currentRoom = rooms[3];  // Start in Room 1

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
                player.setCurrentHP(player.getMaxHP());     // Reset health
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
            if (!font.loadFromFile("arial.ttf")) {
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
        cout << currentRoom->getName() << endl;

        player.update(deltaTime, *currentRoom, window);

        // Game Event 1
        if (!player.getInventory().isEmpty()) {
            for (int i = 0; i < player.getInventory().getSize(); i++) {
                LazerGun* lg = dynamic_cast<LazerGun*>(player.getInventory().getItem(i));
                if (lg && !rooms[1]->areNPCsSpawned()) {
                    rooms[0]->setTempStatus(true);
                    rooms[0]->addObstacle(new HealingObstacle(40, 220, 10, 10));
                    currentRoom->getCharacters().pushBack(new Undead(100, 80, 80));
                    currentRoom->getCharacters().pushBack(new Undead(100, 200, 80));

                    if (!eventSoundEffect.openFromFile("scream.mp3")) {
                        std::cerr << "Failed to load scream.mp3!" << std::endl;
                        return -1;  
                    }
                    eventSoundEffect.setVolume(30);
                    eventSoundEffect.play();

                    currentRoom->setNPCsSpawned(true);
                }
            }
        }
        // Game Event 2
        if (currentRoom == rooms[2]) {
            if (!rooms[2]->areNPCsSpawned()) {
                Undead* undead1 = new Undead(100, 500, 200);
                //rooms[2]->getCharacters().pushBack(undead1);

                // Set the flag to indicate NPCs have been spawned
                rooms[2]->setNPCsSpawned(true);
            }
        }

        // Game Event 3
        /*
        static float killObstacleTimer = 0.0f;  // Timer to track time for width growth
        if (currentRoom == rooms[3]) {
            killObstacleTimer += deltaTime;  // Increment the timer by deltaTime

            // Every second, increase width by 20 units
            if (killObstacleTimer >= 1.0f) {
                auto it = rooms[3]->getObstacles().getIteratorFromFront();
                while (it != it.end()) {
                    KillObstacle* kill = dynamic_cast<KillObstacle*>(it.getCurrent()->getValue());
                    if (kill) {
                        kill->setWidth(kill->getWidth() + 20.0f);  // Increase width by 20 units
                    }
                    ++it;
                }
                killObstacleTimer = 0.0f;  // Reset the timer after the growth
            }
        }
        else {
            // If player leaves Room 3, reset the obstacle width and timer
            killObstacleTimer = 0.0f;  // Reset the timer

            auto it = rooms[3]->getObstacles().getIteratorFromFront();
            while (it != it.end()) {
                KillObstacle* kill = dynamic_cast<KillObstacle*>(it.getCurrent()->getValue());
                if (kill) {
                    kill->setWidth(10.0f);  // Reset width to initial value (10 units)
                }
                ++it;
            }
        }
        */
        // Game Event 4 (FreeBird on Room 5)
        if (currentRoom == rooms[4] && currentMusic != "freebird" && !rooms[4]->getCharacters().isEmpty()) {
            // Load and start playing background music
            if (!backgroundMusic.openFromFile("freebird.mp3")) {
                std::cerr << "Failed to load freebird.mp3!" << std::endl;
                return -1;  // Exit if music fails to load
            }
            backgroundMusic.setLoop(true);    // Enable looping
            backgroundMusic.setVolume(15);    // Set volume (adjust as needed)
            backgroundMusic.play();

            // Set the flag to indicate that the music has been played
            currentMusic = "freebird";
        }

        // If the All Enemies Dies, play lacrimosa
        if (currentRoom == rooms[4] && currentMusic == "freebird") {
            if (rooms[3]->getCharacters().isEmpty()) {
                if (!backgroundMusic.openFromFile("lacrimosa.mp3")) {
                    std::cerr << "Failed to load lacrimosa.mp3!" << std::endl;
                    return -1;  // Exit if music fails to load
                }
                backgroundMusic.setLoop(true);    // Enable looping
                backgroundMusic.setVolume(30);    // Set volume (adjust as needed)
                backgroundMusic.play();

                currentMusic = "lacrimosa";  // Reset the flag when leaving the room
            }
        }

        // If player leaves the room, reset the music flag
        if (currentRoom != rooms[4] && currentMusic == "freebird") {
            if (!backgroundMusic.openFromFile("lacrimosa.mp3")) {
                std::cerr << "Failed to load lacrimosa.mp3!" << std::endl;
                return -1;  // Exit if music fails to load
            }
            backgroundMusic.setLoop(true);    // Enable looping
            backgroundMusic.setVolume(30);    // Set volume (adjust as needed)
            backgroundMusic.play();

            currentMusic = "lacrimosa";  // Reset the flag when leaving the room
        }


        // Game Event 5 (Play boss music when player enters room 3)
        if (currentRoom == rooms[7] && currentMusic != "boss1" && !rooms[7]->getCharacters().isEmpty()) {
            // Load and start playing background music
            if (!backgroundMusic.openFromFile("boss1.mp3")) {
                std::cerr << "Failed to load boss1.mp3!" << std::endl;
                return -1;  // Exit if music fails to load
            }
            backgroundMusic.setLoop(true);    // Enable looping
            backgroundMusic.setVolume(15);    // Set volume (adjust as needed)
            backgroundMusic.play();

            // Set the flag to indicate that the music has been played
            currentMusic = "boss1";
        }

        // If the Boss Dies, play lacrimosa
        if (currentRoom == rooms[7] && currentMusic == "boss1") {
            if (rooms[3]->getCharacters().isEmpty()) {
                if (!backgroundMusic.openFromFile("lacrimosa.mp3")) {
                    std::cerr << "Failed to load lacrimosa.mp3!" << std::endl;
                    return -1;  // Exit if music fails to load
                }
                backgroundMusic.setLoop(true);    // Enable looping
                backgroundMusic.setVolume(30);    // Set volume (adjust as needed)
                backgroundMusic.play();

                currentMusic = "lacrimosa";  // Reset the flag when leaving the room
            }
        }

        // If player leaves the room, reset the music flag
        if (currentRoom != rooms[7] && currentMusic == "boss1") {
            if (!backgroundMusic.openFromFile("lacrimosa.mp3")) {
                std::cerr << "Failed to load lacrimosa.mp3!" << std::endl;
                return -1;  // Exit if music fails to load
            }
            backgroundMusic.setLoop(true);    // Enable looping
            backgroundMusic.setVolume(30);    // Set volume (adjust as needed)
            backgroundMusic.play();

            currentMusic = "lacrimosa";  // Reset the flag when leaving the room
        }

        currentRoom->updateProjectile(deltaTime, player, window); // Method to update Projectile
        window.display();
    }

    return 0;
}

// Method to spawn NPCs
void GameEngine::spawnNPC(Room* currentRoom) {
    // Check if NPCs are already spawned
    if (currentRoom->getName() == "Room 2" && !currentRoom->areNPCsSpawned()) {
        //Undead* undead1 = new Undead(100, 500, 200);
        //currentRoom->getCharacters().pushBack(undead1);

        // Set the flag to indicate NPCs have been spawned
        //currentRoom->setNPCsSpawned(true);
    }

    if (currentRoom->getName() == "Room 5" && !currentRoom->areNPCsSpawned()) {
        Undead* undead1 = new Undead(100, 500, 200);
        currentRoom->getCharacters().pushBack(undead1);

        // Set the flag to indicate NPCs have been spawned
        currentRoom->setNPCsSpawned(true);
        /*
        Juggernaut* boss = new Juggernaut(500, 500, 200);
        currentRoom->getCharacters().pushBack(boss);

        // Set the flag to indicate NPCs have been spawned
        currentRoom->setNPCsSpawned(true);
        */
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
        if (room->getCharacters().isEmpty()) {
            room->resetNPCdead();
            room->setNPCsSpawned(false);  // Reset spawn flag
        }
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

    //room1->addObstacle(new ItemObstacle(300, 125, new LifeTotem()));
    //room1->addObstacle(new HealingObstacle(200, 125, 10, 10)); // Healing Item

    room1->addObstacle(new NormalObstacle(0, 350, 640, 10));  // Floor obstacle
    room1->addObstacle(new NormalObstacle(0, 0, 300, 10));  // Celling obstacle
    room1->addObstacle(new NormalObstacle(340, 0, 300, 10));  // Celling obstacle
    room1->addObstacle(new NormalObstacle(0, 0, 10, 360));  // Left Wall
    room1->addObstacle(new NormalObstacle(630, 0, 10, 300));  // Right Wall
    room1->addObstacle(new NormalObstacle(0, 250, 100, 20)); // Left Platform
    room1->addObstacle(new NormalObstacle(500, 100, 60, 20)); // Top Right Platform

    room1->addTempObstacle(new NormalObstacle(630, 300, 10, 60, sf::Color::Green));

    room1->addObstacle(new MovingObstacle(300, 150, 80, 20, 100.0f, 50.0f, 300.0f)); // moving obstacle

    room1->addObstacle(new KillObstacle(0, 360, 640, 20)); // Add KillObstacle

    return room1;
}

Room* GameEngine::initializeRoom2() {
    Room* room2 = new Room("Room 2");
    room2->addObstacle(new NormalObstacle(0, 350, 300, 10));  // Floor obstacle
    room2->addObstacle(new NormalObstacle(340, 350, 300, 10));  // Floor obstacle
    room2->addObstacle(new NormalObstacle(0, 0, 10, 360));  // Left Wall
    room2->addObstacle(new NormalObstacle(630, 0, 10, 360));  // Right Wall
    room2->addObstacle(new NormalObstacle(0, 0, 640, 10));  // Celling obstacle

    room2->addObstacle(new NormalObstacle(40, 250, 600, 20)); // First Platform
    room2->addObstacle(new NormalObstacle(0, 180, 600, 20)); // Second Platform
    room2->addObstacle(new NormalObstacle(40, 110, 600, 20)); // Third Platform

    room2->addObstacle(new ItemObstacle(600, 90, new LazerGun())); // The LazerGun Object

    return room2;
}

Room* GameEngine::initializeRoom3() {
    Room* room3 = new Room("Room 3");

    room3->addObstacle(new ItemObstacle(580, 40, new LifeTotem())); // Life totem Item

    room3->addObstacle(new NormalObstacle(0, 350, 300, 10));  // Floor obstacle
    room3->addObstacle(new NormalObstacle(340, 350, 300, 10));  // Floor obstacle
    room3->addObstacle(new NormalObstacle(0, 0, 560, 10));  // Celling obstacle
    room3->addObstacle(new NormalObstacle(630, 0, 10, 10));  // Celling obstacle

    room3->addObstacle(new NormalObstacle(540, 60, 100, 10)); // Top Left obstacle

    room3->addObstacle(new NormalObstacle(0, 0, 10, 300));  // Left Wall
    room3->addObstacle(new NormalObstacle(630, 60, 10, 300));  // Right Wall

    return room3;
}

Room* GameEngine::initializeRoom4() {
    Room* room4 = new Room("Room 4");
    room4->addObstacle(new NormalObstacle(0, 350, 640, 10));  // Floor obstacle
    room4->addObstacle(new NormalObstacle(0, 0, 300, 10));  // Celling obstacle
    room4->addObstacle(new NormalObstacle(340, 0, 300, 10));  // Celling obstacle
    room4->addObstacle(new NormalObstacle(630, 0, 10, 300));  // Right Wall

    room4->addObstacle(new NormalObstacle(340, 10, 10, 100)); 
    room4->addObstacle(new NormalObstacle(60, 100, 280, 10));
    room4->addObstacle(new NormalObstacle(0, 180, 540, 10));
    room4->addObstacle(new NormalObstacle(60, 290, 600, 10));

    room4->addObstacle(new KillObstacle(0, 0, 20, 360));  // Left Kill Wall
    room4->addObstacle(new KillObstacle(460, 120, 20, 60));
    room4->addObstacle(new KillObstacle(520, 10, 20, 100));
    room4->addObstacle(new KillObstacle(460, 270, 20, 20));
    room4->addObstacle(new KillObstacle(360, 270, 20, 20));
    room4->addObstacle(new KillObstacle(260, 270, 20, 20));

    return room4;
}

Room* GameEngine::initializeRoom5() {
    Room* room5 = new Room("Room 5");

    room5->addObstacle(new NormalObstacle(0, 350, 640, 10));  // Floor obstacle

    room5->addObstacle(new NormalObstacle(0, 0, 300, 10));  // Celling obstacle
    room5->addObstacle(new NormalObstacle(340, 0, 300, 10));  // Celling obstacle
    return room5;
}

Room* GameEngine::initializeRoom6() {
    Room* room5 = new Room("Room 6");
    return room5;
}

Room* GameEngine::initializeRoom7() {
    Room* room5 = new Room("Room 7");
    return room5;
}

Room* GameEngine::initializeRoom8() {
    Room* room5 = new Room("Room 8");
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

void GameEngine::handleRoomTransitions(Player& player, Room*& currentRoom, List<Room*>& rooms, bool& isRespawning) {
    if (isRespawning) return;

    sf::FloatRect playerBounds = player.getBounds();

    // Room 1 transitions
    if (currentRoom == rooms[0]) {  // Room 1
        if (playerBounds.top < 0) {  // Up to Room 2
            player.setPosition(player.getPosition().x, 360 - playerBounds.height);
            currentRoom = rooms[1];
            respawnNPC();
        }
        else if (playerBounds.left + playerBounds.width > 640) {  // Right to Room 3
            player.setPosition(0, player.getPosition().y);
            currentRoom = rooms[2];
            respawnNPC();
        }
    }
    // Room 2 transitions
    else if (currentRoom == rooms[1]) {  // Room 2
        if (playerBounds.top + playerBounds.height > 360) {  // Down to Room 1
            player.setPosition(player.getPosition().x, 0);
            currentRoom = rooms[0];
            respawnNPC();
        }
    }
    // Room 3 transitions
    else if (currentRoom == rooms[2]) {  // Room 3
        if (playerBounds.left < 0) {  // Left to Room 1
            player.setPosition(640 - playerBounds.width, player.getPosition().y);
            currentRoom = rooms[0];
            respawnNPC();
        }
        else if (playerBounds.left + playerBounds.width > 640) {  // Right to Room 6
            player.setPosition(0, player.getPosition().y);
            currentRoom = rooms[5];
            respawnNPC();
        }
        else if (playerBounds.top < 0) {  // Up to Room 7
            player.setPosition(player.getPosition().x, 360 - playerBounds.height);
            currentRoom = rooms[6];
            respawnNPC();
        }
        else if (playerBounds.top + playerBounds.height > 360) {  // Down to Room 4
            player.setPosition(player.getPosition().x, 0);
            currentRoom = rooms[3];
            respawnNPC();
        }
    }
    // Room 4 transitions
    else if (currentRoom == rooms[3]) {  // Room 4
        if (playerBounds.top < 0) {  // Up to Room 3
            player.setPosition(player.getPosition().x, 360 - playerBounds.height);
            currentRoom = rooms[2];
            respawnNPC();
        }
        else if (playerBounds.left + playerBounds.width > 640) {  // Right to Room 5
            player.setPosition(0, player.getPosition().y);
            currentRoom = rooms[4];
            respawnNPC();
        }
    }
    // Room 5 transitions
    else if (currentRoom == rooms[4]) {  // Room 5
        if (playerBounds.left < 0) {  // Left to Room 4
            player.setPosition(640 - playerBounds.width, player.getPosition().y);
            currentRoom = rooms[3];
            respawnNPC();
        }
        else if (playerBounds.top < 0) {  // Up to Room 6
            player.setPosition(player.getPosition().x, 360 - playerBounds.height);
            currentRoom = rooms[5];
            respawnNPC();
        }
    }
    // Room 6 transitions
    else if (currentRoom == rooms[5]) {  // Room 6
        if (playerBounds.left < 0) {  // Left to Room 3
            player.setPosition(640 - playerBounds.width, player.getPosition().y);
            currentRoom = rooms[2];
            respawnNPC();
        }
        else if (playerBounds.top + playerBounds.height > 360) {  // Down to Room 5
            player.setPosition(player.getPosition().x, 0);
            currentRoom = rooms[4];
            respawnNPC();
        }
    }
    // Room 7 transitions
    else if (currentRoom == rooms[6]) {  // Room 7
        if (playerBounds.top + playerBounds.height > 360) {  // Down to Room 3
            player.setPosition(player.getPosition().x, 0);
            currentRoom = rooms[2];
            respawnNPC();
        }
        else if (playerBounds.left + playerBounds.width > 640) {  // Right to Room 8
            player.setPosition(0, player.getPosition().y);
            currentRoom = rooms[7];
            respawnNPC();
        }
    }
    // Room 8 transitions
    else if (currentRoom == rooms[7]) {  // Room 8
        if (playerBounds.left < 0) {  // Left to Room 7
            player.setPosition(640 - playerBounds.width, player.getPosition().y);
            currentRoom = rooms[6];
            respawnNPC();
        }
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

