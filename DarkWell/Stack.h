#pragma once
#include "List.h"
#include <stdexcept>

template<class T>
class Stack {
private:
    List<T> list;

public:
    Stack() {}

    // Push a new item onto the stack
    void push(const T& value) {
        list.pushBack(value);
    }

    // Remove and return the top item from the stack
    T pop() {
        if (isEmpty()) {
            throw std::out_of_range("Stack is empty");
        }
        return list.popBack();
    }

    // Peek at the bottom item without removing it
    T& bottom() {
        if (isEmpty()) {
            throw std::out_of_range("Stack is empty");
        }
        return list[0];
    }

    // Peek at the top item without removing it
    T& top() {
        if (isEmpty()) {
            throw std::out_of_range("Stack is empty");
        }
        return list[list.size() - 1];
    }

    // Peek at the top item without removing it (const version)
    const T& top() const {
        if (isEmpty()) {
            throw std::out_of_range("Stack is empty");
        }
        return list[list.size() - 1];
    }

    // Check if the stack is empty
    bool isEmpty() const {
        return list.isEmpty();
    }

    // Get the current size of the stack
    int size() const {
        return list.size();
    }

    // Clear the stack
    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }
};
