#pragma once
#include "List.h"
#include <stdexcept>

template<class T>
class Stack {
private:
    List<T> list;
    int maxSize;

public:
    // Limit Stack size to 5
    Stack(int maxSize = 5) : maxSize(maxSize) {}

    // push a new item; popback if the max size is reached
    void push(const T& value) {
        if (list.size() >= maxSize) {
            throw std::out_of_range("Stack is full");
        }
        list.pushBack(value);
    }

    // pop the first item
    T pop() {
        if (isEmpty()) {
            throw std::out_of_range("Stack is empty");
        }
        return list.popBack();
    }

    // Peek the front item without removing it
    T& front() {
        if (isEmpty()) {
            throw std::out_of_range("Stack is empty");
        }
        return list[0];
    }

    // Peek the back item without removing it
    T& back() {
        if (isEmpty()) {
            throw std::out_of_range("Stack is empty");
        }
        return list[list.size() - 1];
    }

    // Check if the Stack is empty
    bool isEmpty() const {
        return list.isEmpty();
    }

    // Get the current size of the Stack
    int size() const {
        return list.size();
    }
};
