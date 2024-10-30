#pragma once

template<class DataType>
class DoublyLinkedNode
{
public:
    typedef DoublyLinkedNode<DataType> Node;
    static Node NIL;

private:
    DataType value;
    Node* next;
    Node* previous;

public:
    DoublyLinkedNode()
    {
        value = DataType();
        next = &NIL;
        previous = &NIL;
    }

    DoublyLinkedNode(const DataType& aValue)
    {
        value = aValue;
        next = &NIL;
        previous = &NIL;
    }

    // Add a node before the current one. (New Node is nearer the Head)
    void prepend(Node* newNode)
    {
        newNode->next = this;

        if (this->previous != &NIL)
        {
            newNode->previous = this->previous;
            this->previous->next = newNode;
        }

        this->previous = newNode;
    }

    // Add a node after the current one. (Current Node is nearer the Head)
    void append(Node* newNode)
    {
        newNode->previous = this;

        if (this->next != &NIL)
        {
            newNode->next = this->next;
            this->next->previous = newNode;
        }

        this->next = newNode;
    }

    // Delete the current node from the list. Pay attention to the sequence.
    void remove()
    {
        if (this->previous != &NIL)
        {
            this->previous->next = this->next;
        }

        if (this->next != &NIL)
        {
            this->next->previous = this->previous;
        }

        delete this;
    }

    DataType getValue() const
    {
        return value;
    }

    Node* getNext() const
    {
        return next;
    }

    Node* getPrevious() const
    {
        return previous;
    }
};

template<class DataType>
DoublyLinkedNode<DataType> DoublyLinkedNode<DataType>::NIL;

