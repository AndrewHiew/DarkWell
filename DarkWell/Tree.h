#pragma once
#include "TreeNode.h"

template <typename T>
class Tree {
private:
    TreeNode<T>* root;

public:
    // Static NIL object for Tree reference
    static TreeNode<T> NIL;

    Tree() : root(nullptr) {}

    void setRoot(TreeNode<T>* newRoot) {
        root = newRoot;
    }

    TreeNode<T>* getRoot() const {
        return root;
    }

    // Declaration of findRoomNode
    TreeNode<T>* findRoomNode(T room);

private:
    // Recursive function to find the room node
    TreeNode<T>* findRoomNodeRecursive(TreeNode<T>* node, T room);
};

// Implementation of findRoomNode in the Tree class
template <typename T>
TreeNode<T>* Tree<T>::findRoomNode(T room) {
    return findRoomNodeRecursive(root, room);
}

// Recursive function to find the room node
template <typename T>
TreeNode<T>* Tree<T>::findRoomNodeRecursive(TreeNode<T>* node, T room) { // Fixed to be a member function
    if (!node || node == &Tree::NIL) { // Use Tree::NIL correctly
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

// Initialize the static NIL node for Tree
template <typename T>
TreeNode<T> Tree<T>::NIL;
