#pragma once
#include <SFML/Graphics.hpp>

class GameStateManager {
private:
    bool isPlayerDead;
    bool showDialog;

public:
    GameStateManager();

    void setPlayerDead();
    bool isDead() const;
    void resetGame();
    bool shouldShowDialog() const;
    void drawDeathDialog(sf::RenderWindow& window) const;
};
