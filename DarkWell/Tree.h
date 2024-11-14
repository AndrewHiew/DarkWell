#pragma once
#include <functional>
#include <stdexcept>
#include <iostream>

template<class T, int N>
class NTree {
private:
    const T* fKey;
    NTree<T, N>* fNodes[N];
    std::function<void(float)> fAction; // Updated to accept float parameter

    // Private default constructor
    NTree() : fKey(nullptr), fAction(nullptr) {
        for (int i = 0; i < N; i++) {
            fNodes[i] = &NIL;
        }
    }

public:
    static NTree<T, N> NIL;

    // Updated constructor
    NTree(const T& aKey, std::function<void(float)> aAction = nullptr)
        : fKey(&aKey), fAction(aAction) {
        for (int i = 0; i < N; i++) {
            fNodes[i] = &NIL;
        }
    }

    ~NTree() {
        for (int i = 0; i < N; i++) {
            if (fNodes[i] != &NIL) {
                delete fNodes[i];
            }
        }
    }

    bool isEmpty() const { return this == &NIL; }

    const T& getKey() const {
        if (isEmpty()) throw std::domain_error("Empty NTree");
        return *fKey;
    }

    void attachNTree(int aIndex, NTree<T, N>* aNTree) {
        if (isEmpty()) throw std::domain_error("Empty NTree");
        if ((aIndex >= 0) && (aIndex < N)) {
            if (fNodes[aIndex] != &NIL) throw std::domain_error("Non-empty subtree present");
            fNodes[aIndex] = aNTree;
        }
    }

    NTree<T, N>& operator[](int aIndex) const {
        if (isEmpty()) throw std::domain_error("Empty NTree");
        if ((aIndex >= 0) && (aIndex < N)) {
            return *fNodes[aIndex];
        }
        throw std::out_of_range("Illegal subtree index");
    }

    NTree<T, N>* detachNTree(int aIndex) {
        if (isEmpty()) throw std::domain_error("Empty NTree!");
        if ((aIndex >= 0) && (aIndex < N)) {
            NTree<T, N>* Result = fNodes[aIndex];
            fNodes[aIndex] = &NIL;
            return Result;
        }
        throw std::out_of_range("Illegal subtree index");
    }

    // Updated to execute action with float parameter
    void executeAction(float deltaTime) {
        if (isEmpty()) throw std::domain_error("Empty NTree!");

        if (fAction) {
            fAction(deltaTime); // Call the action with deltaTime
        }
        else {
            std::cerr << "No action defined for this node.\n";
        }
    }

    void executeAction(int aIndex, float deltaTime) {
        if (isEmpty()) throw std::domain_error("Empty NTree!");

        if (aIndex < 0 || aIndex >= N) {
            throw std::out_of_range("Illegal subtree index");
        }

        if (fNodes[aIndex] != &NIL) {
            fNodes[aIndex]->executeAction(deltaTime);
        }
        else {
            std::cerr << "No action to execute at index " << aIndex << std::endl;
        }
    }
};

// Define the static NIL node
template<class T, int N>
NTree<T, N> NTree<T, N>::NIL;
