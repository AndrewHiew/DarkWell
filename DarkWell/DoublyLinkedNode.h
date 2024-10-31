#pragma once

template<class DataType>
class DoublyLinkedNode {
public:
    typedef DoublyLinkedNode<DataType> Node;
    static Node NIL;

private:
    DataType value;
    Node* next;
    Node* previous;

public:
    DoublyLinkedNode() : value(DataType()), next(&NIL), previous(&NIL) {}

    DoublyLinkedNode(const DataType& aValue) : value(aValue), next(&NIL), previous(&NIL) {}

    // Add a node before the current one.
    void prepend(Node* newNode) {
        newNode->next = this;

        if (this->previous != &NIL) {
            newNode->previous = this->previous;
            this->previous->next = newNode;
        }

        this->previous = newNode;
    }

    // Add a node after the current one.
    void append(Node* newNode) {
        newNode->previous = this;

        if (this->next != &NIL) {
            newNode->next = this->next;
            this->next->previous = newNode;
        }

        this->next = newNode;
    }

    // Delete the current node from the list.
    void remove() {
        if (this->previous != &NIL) {
            this->previous->next = this->next;
        }

        if (this->next != &NIL) {
            this->next->previous = this->previous;
        }

        delete this;
    }

    // Return value by reference
    DataType& getValue() { return value; }
    const DataType& getValue() const { return value; }

    Node* getNext() const { return next; }
    Node* getPrevious() const { return previous; }
};

// Initialize the static NIL node
template<class DataType>
DoublyLinkedNode<DataType> DoublyLinkedNode<DataType>::NIL;
