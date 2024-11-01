#pragma once

// Singly linked list Node template
template <typename T>
class Node {
public:
    T value;
    Node* next;

    Node(T val) : value(val), next((Node*)0) {}
};
