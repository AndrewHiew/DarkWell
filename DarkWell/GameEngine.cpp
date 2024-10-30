#include <SFML/Graphics.hpp>
#include "GameEngine.h"


GameEngine::GameEngine() {

}

GameEngine::~GameEngine() {

}

int GameEngine::StartGame() {
    sf::RenderWindow window(sf::VideoMode(640, 360), "DarkWell - Room with Obstacles");
    window.setFramerateLimit(60);

    Player player(100);  // Create a player with 100 HP
    Room room;  // Create a room

    // Add a floor obstacle (covers the bottom of the screen)
    room.addObstacle(Obstacle(0, 320, 640, 40));

    // Add a few floating obstacles
    room.addObstacle(Obstacle(300, 250, 100, 20));
    room.addObstacle(Obstacle(100, 200, 100, 20));
    room.addObstacle(Obstacle(300, 150, 80, 20));
    room.addObstacle(Obstacle(500, 100, 60, 20));

    sf::Clock clock;  // For deltaTime calculations

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();  // Calculate deltaTime

        player.handleInput();  // Handle keyboard input
        player.update(deltaTime, room);  // Update player with room collision

        window.clear(sf::Color::White);  // Clear the window
        room.draw(window);  // Draw the room and its obstacles
        player.draw(window);  // Draw the player
        window.display();  // Display everything
    }


    return 0;
}
