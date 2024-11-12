#pragma once
#include "List.h"
#include <stdexcept>

template<class T>
class Queue {
private:
    List<T> list;
    int maxSize;

public:
    Queue(int maxSize = 5) : maxSize(maxSize) {}

    // Enqueue a new item; dequeue if the max size is reached
    void enqueue(const T& value) {
        if (list.size() >= maxSize) {
            dequeue();  // Dequeue the oldest item if at max capacity
        }
        list.pushBack(value);
    }

    // Dequeue the front item
    T dequeue() {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty");
        }
        return list.popFront();
    }

    // Peek the front item without removing it
    T& front() {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty");
        }
        return list[0];
    }

    // Peek the back item without removing it
    T& back() {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty");
        }
        return list[list.size() - 1];
    }

    // Check if the queue is empty
    bool isEmpty() const {
        return list.isEmpty();
    }

    // Get the current size of the queue
    int size() const {
        return list.size();
    }
};
