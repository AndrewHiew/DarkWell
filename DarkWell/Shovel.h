#pragma once
#include "Item.h"
#include <iostream>

class Shovel : public Item {
public:
    Shovel() {
        color = sf::Color(139, 69, 19);  // Brown for Shovel
    }

    std::string getName() const override { return "Shovel"; }
    void use() const override { std::cout << "Digging with the Shovel.\n"; }
};
