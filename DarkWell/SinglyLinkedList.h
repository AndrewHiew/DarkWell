#pragma once
#include "Node.h"
#include <stdexcept>
#include "NodeIterator.h"  // Include NodeIterator

template <typename T>
class SinglyLinkedList {
private:
    Node<T>* head;
    int size;

public:
    SinglyLinkedList() : head(nullptr), size(0) {}
    ~SinglyLinkedList() {
        clear();  // Clean up memory
    }

    void pushBack(T value) {
        Node<T>* newNode = new Node<T>(value);
        if (!head) {
            head = newNode;
        }
        else {
            Node<T>* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
        size++;
    }

    void popFront() {
        if (!head) throw std::out_of_range("List is empty");
        Node<T>* temp = head;
        head = head->next;
        delete temp;
        size--;
    }

    // Add the operator[] to access elements
    T& operator[](int index) {
        if (index < 0 || index >= size) throw std::out_of_range("Invalid index");
        Node<T>* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->value;
    }

    int getSize() const {
        return size;
    }

    bool isEmpty() const {
        return size == 0;
    }

    void clear() {
        while (!isEmpty()) {
            popFront();
        }
    }

    Node<T>* getHead() const {
        return head;
    }

    // Method to set a new head of the list
    void setHead(Node<T>* newHead) {
        head = newHead;
        // If we set a new head, we need to update the size if the new head is not null
        if (newHead == nullptr) {
            size = 0;
        }
        else {
            // Recalculate size
            int newSize = 0;
            Node<T>* current = newHead;
            while (current != nullptr) {
                newSize++;
                current = current->next;
            }
            size = newSize;
        }
    }

    // Method to return an iterator pointing to the start of the list
    NodeIterator<T> getIterator() {
        return NodeIterator<T>(head);
    }
};
