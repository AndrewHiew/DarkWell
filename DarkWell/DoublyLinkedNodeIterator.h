#pragma once

#include "DoublyLinkedNode.h"

template<class DataType>
class DoublyLinkedNodeIterator 
{
private:
    typedef DoublyLinkedNode<DataType> Node;
    Node* current;

public:
    DoublyLinkedNodeIterator(Node* start) : current(start) {}
    DoublyLinkedNodeIterator(Node& node) : current(&node) {}  

    // Pre-increment
    DoublyLinkedNodeIterator& operator++() 
    {
        current = current->getNext();
        return *this;
    }

    // Post-increment
    DoublyLinkedNodeIterator& operator++(int) 
    {
        DoublyLinkedNodeIterator& temp = *this;
        ++(*this);
        return temp;
    }

    // Pre-decrement
    DoublyLinkedNodeIterator& operator--() 
    {
        current = current->getPrevious();
        return *this;
    }

    // Post-decrement
    DoublyLinkedNodeIterator& operator--(int) 
    {
        DoublyLinkedNodeIterator& temp = *this;
        --(*this);
        return temp;
    }

    bool operator==(const DoublyLinkedNodeIterator& other) const 
    {
        return current == other.current;
    }

    bool operator!=(const DoublyLinkedNodeIterator& other) const 
    {
        return current != other.current;
    }

    DataType operator*() const 
    {
        return current->getValue();
    }

    Node* getCurrent() 
    {
        return current;
    }

    DoublyLinkedNodeIterator begin() 
    {
        return DoublyLinkedNodeIterator(current);
    }

    DoublyLinkedNodeIterator end() 
    {
        return DoublyLinkedNodeIterator(Node::NIL);
    }
};

