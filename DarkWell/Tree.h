#pragma once
#include "TreeNode.h"
#include "TreeIterator.h"

template <typename T>
class Tree {
private:
    TreeNode<T>* root;  // Pointer to the root of the tree

public:
    // Constructor
    Tree() : root(&TreeNode<T>::NIL) {}

    // Set the root of the tree
    void setRoot(TreeNode<T>* rootNode) {
        root = rootNode ? rootNode : &TreeNode<T>::NIL;
    }

    // Get the root of the tree
    TreeNode<T>* getRoot() const {
        return root;
    }

    // Destructor
    ~Tree() {
        delete root;  // This will call the TreeNode destructor, which deletes children
    }
};
