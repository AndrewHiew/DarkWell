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
    TreeNode<T>* parent;                       // Pointer to the parent node
    static const int MAX_CHILDREN = 4;

public:
    // Static NIL object for TreeNode reference
    static TreeNode<T> NIL;

    // Default constructor
    TreeNode() : value(T()), parent(nullptr) {}

    // Constructor with value and parent
    TreeNode(const T& value, TreeNode<T>* parentNode = nullptr) 
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
};

// Initialize the static NIL node
template <typename T>
TreeNode<T> TreeNode<T>::NIL;
