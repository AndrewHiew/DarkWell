#pragma once
#include "Item.h"
#include <iostream>


class ItemVisitor;

class Shovel : public Item {
public:
    Shovel() {
        color = sf::Color(139, 69, 19);
        imagePath = "lifetotem.png";
    }

    std::string getName() const override { return "Shovel"; }
    void use() const override { std::cout << "Digging with the Shovel.\n"; }

    virtual void accept(ItemVisitor& v) {
        v.Visit(*this);
    }
};
