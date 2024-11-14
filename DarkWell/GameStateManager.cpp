#pragma once
#include "GameStateManager.h"
#include <SFML/Graphics.hpp>


GameStateManager::GameStateManager() : isPlayerDead(false), showDialog(false) {}

void GameStateManager::setPlayerDead() {
    isPlayerDead = true;
    showDialog = true;
}

bool GameStateManager::isDead() const { return isPlayerDead; }

void GameStateManager::resetGame() {
    isPlayerDead = false;
    showDialog = false;
}

bool GameStateManager::shouldShowDialog() const { return showDialog; }

void GameStateManager::drawDeathDialog(sf::RenderWindow& window) const {
    if (!showDialog) return;

    sf::Font font;
    font.loadFromFile("Arial.ttf");

    sf::Text text("You Died! Press R to Respawn", font, 30);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    text.setPosition(window.getSize().x / 2 - 150, window.getSize().y / 2);

    window.draw(text);
}