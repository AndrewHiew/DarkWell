#pragma once
#include "Node.h"

// Singly linked list Node Iterator template
template <typename T>
class NodeIterator {
private:
    Node<T>* current;

public:
    NodeIterator(Node<T>* startNode) : current(startNode) {}

    T& operator*() { return current->value; }

    NodeIterator& operator++() {
        current = current->next;
        return *this;
    }

    bool operator!=(const NodeIterator& other) const {
        return current != other.current;
    }

    bool operator==(const NodeIterator& other) const {
        return current == other.current;
    }

    NodeIterator end() const { return NodeIterator((Node<T>*)0); }
};
