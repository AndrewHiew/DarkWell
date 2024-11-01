#pragma once
#include "SinglyLinkedList.h"  // Ensure to include the correct header for SinglyLinkedList
#include "Item.h"

class Inventory {
private:
    SinglyLinkedList<Item*> items;  // Use your SinglyLinkedList for items

public:
    ~Inventory() {
        // Automatically handled by SinglyLinkedList destructor
        clear();  // Optional: you can clear items in destructor
    }

    void addItem(Item* item) { items.pushBack(item); }

    void removeItem(int index) {
        if (index < 0 || index >= items.getSize()) throw std::out_of_range("Invalid index");

        // This part can be tricky since removing from a singly linked list requires traversal
        if (index == 0) {
            items.popFront();
        }
        else {
            Node<Item*>* current = items.getHead();
            for (int i = 0; i < index - 1; ++i) {
                current = current->next;  // Traverse to the previous node
            }
            Node<Item*>* temp = current->next;  // Node to remove
            current->next = current->next->next; // Bypass the removed node
            delete temp->value; // Free the memory for the item
            delete temp; // Free the node memory
        }
    }

    Item* getItem(int index) const {
        if (index < 0 || index >= items.getSize()) throw std::out_of_range("Invalid index");

        Node<Item*>* current = items.getHead();
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->value;  // Return the item pointer
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
