#pragma once

// Singly linked list Node template
template <typename T>
class Node {
public:
    T value;
    Node* next;

    Node(T val) : value(val), next(nullptr) {}

    // This method returns the value stored in the node
    T& getValue() {
        return value;
    }

    // Optional: const version of getValue for read-only access
    const T& getValue() const {
        return value;
    }
};
