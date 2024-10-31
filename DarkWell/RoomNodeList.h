#pragma once
#include "Node.h"
#include "NodeIterator.h"

template <typename T>
class RoomNodeList {
private:
    Node<T>* head;            // Head of the linked list of nodes
    Node<T>* tail;            // Tail for easy access
    int count;                // Node count

public:
    RoomNodeList() : head((Node<T>*)0), tail((Node<T>*)0), count(0) {}

    // Add a RoomNode to the list
    void addRoomNode(T* roomNode) {
        Node<T>* newNode = new Node<T>(roomNode);
        if (head == (Node<T>*)0) {
            head = newNode;
            tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
        ++count;
    }

    // Get an iterator from the beginning of the list
    NodeIterator<T> begin() const { return NodeIterator<T>(head); }

    // Get an iterator representing the end of the list
    NodeIterator<T> end() const { return NodeIterator<T>((Node<T>*)0); }

    int size() const { return count; }
};
