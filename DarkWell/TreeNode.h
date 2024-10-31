#pragma once
#include "List.h"
#include "TreeIterator.h"
#include <stdexcept>
#include <iostream> // Include for std::cout

template <typename T>
class TreeNode {
private:
    T value;                                   // The value stored in the node
    List<TreeNode<T>*> children;               // List to hold pointers to children
    TreeNode<T>* parent;                       // Pointer to the parent 
    

public:
    // Static NIL object for TreeNode reference
    static TreeNode<T> NIL;
    static const int MAX_CHILDREN = 4;

    // Default constructor
    TreeNode() : value(T()), parent(&NIL) {}

    // Constructor with value and parent
    TreeNode(const T& value, TreeNode<T>* parentNode = &NIL)
        : value(value), parent(parentNode) {}

    // TreeNode Destructor
    ~TreeNode() {
        // Delete all child nodes to prevent memory leaks
        while (!children.isEmpty()) {
            TreeNode<T>* child = children.popBack(); // Get the next child node
            if (child != &NIL) { // Ensure it's not the NIL node or null
                std::cout << "Deleting child node with value: " << child->getValue() << std::endl;
                delete child;                             // Delete the child
            }
        }
    }

    TreeNode<T>* findRoomNodeRecursive(TreeNode<T>* node, Room* room) {
        if (!node || node == &TreeNode<T*>::NIL) {
            return nullptr; // Base case: node is null or NIL
        }

        // Check if the current node's value matches the room we're looking for
        if (node->getValue() == room) {
            return node; // Found the node
        }

        // Recursively search through children
        const auto& children = node->getChildren();
        for (TreeNode<T>* child : children) {
            TreeNode<T>* foundNode = findRoomNodeRecursive(child, room);
            if (foundNode) {
                return foundNode; // Found the room in one of the children
            }
        }

        return nullptr; // Room not found in this branch
    }


    // Get the value of the node
    T getValue() const {
        return value;
    }

    // Get the parent of the node
    TreeNode<T>* getParent() const {
        return parent;
    }

    // Add a child to the node
    void addChild(TreeNode<T>* child) {
        if (children.size() >= MAX_CHILDREN) { // Check if the limit is reached
            throw std::out_of_range("Cannot add more than 4 children to a TreeNode.");
        }
        child->parent = this; // Set this node as the parent of the child
        children.pushBack(child); // Add the child to the list
        std::cout << "Added child with value: " << child->getValue() << std::endl; // Debug info
    }

    // Get the children of the node
    const List<TreeNode<T>*>& getChildren() const {
        return children;
    }

    // Access child by index
    TreeNode<T>* operator[](size_t index) {
        if (index >= children.size()) {
            throw std::out_of_range("Index out of bounds for TreeNode children.");
        }
        return children[index]; // Return the child at the given index
    }
};

// Initialize the static NIL node
template <typename T>
TreeNode<T> TreeNode<T>::NIL;