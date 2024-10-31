#pragma once

#include "DoublyLinkedNode.h"
#include "DoublyLinkedNodeIterator.h"
#include <stdexcept>

template<class T>
class List {
private:
    typedef DoublyLinkedNode<T> Node;
    Node* last;
    Node* first;
    int count;

public:
    typedef DoublyLinkedNodeIterator<T> Iterator;

    List() : last(&(Node::NIL)), first(&(Node::NIL)), count(0) {}
    List(Node* lastNode) : last(lastNode), first(lastNode), count(1) {}

    ~List() {
        Iterator it = getIteratorFromBack();
        while (it != it.end()) {
            Node* toDelete = it.getCurrent();
            ++it;
            toDelete->remove();
        }
    }

    // Push to the back of the list
    void pushBack(const T& value) {
        Node* newNode = new Node(value);
        if (last != &(Node::NIL)) {
            last->append(newNode);
        }
        else {
            first = newNode;
        }
        last = newNode;
        ++count;
    }

    // Pop from the back of the list
    const T popBack() {
        if (isEmpty()) throw std::out_of_range("List is empty");

        T savedValue = last->getValue();
        Node* toDelete = last;
        last = last->getPrevious();

        if (last == &(Node::NIL)) {
            first = &(Node::NIL);
        }

        toDelete->remove();
        --count;
        return savedValue;
    }

    // Push to the front of the list
    void pushFront(const T& value) {
        Node* newNode = new Node(value);
        if (first != &(Node::NIL)) {
            first->prepend(newNode);
        }
        else {
            last = newNode;
        }
        first = newNode;
        ++count;
    }

    // Pop from the front of the list
    const T popFront() {
        if (isEmpty()) throw std::out_of_range("List is empty");

        T savedValue = first->getValue();
        Node* toDelete = first;
        first = first->getNext();

        if (first == &(Node::NIL)) {
            last = &(Node::NIL);
        }

        toDelete->remove();
        --count;
        return savedValue;
    }

    // Access elements by index (non-const version)
    T& operator[](int index) {
        if (index < 0 || index >= count) {
            throw std::out_of_range("Index out of range");
        }

        Node* current = first;  // Start at the head of the list
        for (int i = 0; i < index; ++i) {
            current = current->getNext();  // Traverse to the node at the index
        }
        return current->getValue();  // Return the reference to the value at the given index
    }

    // Access elements by index (const version)
    const T& operator[](int index) const {
        if (index < 0 || index >= count) {
            throw std::out_of_range("Index out of range");
        }

        Node* current = first;  // Start at the head of the list
        for (int i = 0; i < index; ++i) {
            current = current->getNext();  // Traverse to the node at the index
        }
        return current->getValue();  // Return the reference to the value at the given index
    }

    // Helper Functions
    bool isEmpty() const { return count == 0; }
    int size() const { return count; }

    Iterator getIteratorFromFront() const { return Iterator(first); }
    Iterator getIteratorFromBack() const { return Iterator(last); }
};
