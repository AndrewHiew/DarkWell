#pragma once
#include "SinglyLinkedList.h"  // Ensure to include the correct header for SinglyLinkedList
#include "LifeTotem.h"
#include "Item.h"

class Inventory {
private:
    SinglyLinkedList<Item*> items;  // Use your SinglyLinkedList for items

public:
    ~Inventory() {
        // Automatically handled by SinglyLinkedList destructor
        clear();
    }

    void addItem(Item* item) { items.pushBack(item); }

    void removeItem(int index) {
        if (index < 0 || index >= items.getSize()) throw std::out_of_range("Invalid index");

        if (index == 0) {
            items.popFront();
        }
        else {
            Node<Item*>* current = items.getHead();
            for (int i = 0; i < index - 1; ++i) {
                current = current->next;
            }
            Node<Item*>* temp = current->next; // Node to remove
            current->next = current->next->next; // Bypass the removed node

            delete temp->value;  // Free the memory for the item
            temp->value = nullptr; // Mark value as nullptr (important)
            temp->next = nullptr; // Mark next as nullptr
            delete temp;  // Free the node memory
        }
        items.setHead(items.getHead()); // Update size correctly
    }


    Item* getItem(int index) const {
        if (index < 0 || index >= items.getSize()) throw std::out_of_range("Invalid index");

        Node<Item*>* current = items.getHead();
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }

        // Return nullptr if the value was marked as such
        if (current->value == nullptr) return nullptr;

        return current->value;
    }


    bool findTotem() {
        auto it = items.getHead(); // Using iterator to avoid index access issues
        int index = 0;

        while (it != nullptr) {
            if (dynamic_cast<LifeTotem*>(it->value) != nullptr) {
                // Remove and delete immediately
                removeItem(index);
                return true;
            }
            it = it->next;
            index++;
        }

        return false;
    }


    int getSize() const { return items.getSize(); }

    void useItem(int index) const {
        Item* item = getItem(index);
        item->use();
    }

    bool isEmpty() const {
        return items.isEmpty();
    }

    void clear() {
        while (!isEmpty()) {
            removeItem(0);
        }
    }
};