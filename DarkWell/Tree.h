#pragma once
#include <stdexcept>

template<class T, int N>
class NTree
{
private:
	const T* fKey;
	NTree<T, N>* fNodes[N];

	NTree() : fKey((T*)0)
	{
		for (int i = 0; i < N; i++) {
			fNodes[i] = &NIL;
		}
	}

public:
	static NTree<T, N> NIL;
	NTree(const T& aKey) : fKey(&aKey)
	{
		for (int i = 0; i < N; i++) {
			fNodes[i] = &NIL;
		}
	}
	~NTree()
	{
		for (int i = 0; i < N; i++) {
			if (fNodes[i] != &NIL) {
				delete fNodes[i];
			}
		}
	}

	bool isEmpty() const
	{
		return this == &NIL;
	}

	const T& getKey() const
	{
		if (isEmpty()) {
			throw std::domain_error("Empty NTree");
		}
		return *fKey;
	}

	void attachNTree(int aIndex, NTree<T, N>* aNTree)
	{
		if (isEmpty()) {
			throw std::domain_error("Empty NTree");
		}
		if ((aIndex >= 0) && (aIndex < N)) {
			if (fNodes[aIndex] != &NIL) {
				throw std::domain_error("Non-empty subtree present");
			}
			fNodes[aIndex] = aNTree;
		}
	}

	NTree<T, N>& operator[](int aIndex) const
	{
		if (isEmpty()) {
			throw std::domain_error("Empty NTree");
		}
		if ((aIndex >= 0) && (aIndex < N)) {
			return *fNodes[aIndex];
		}
		else {
			throw std::out_of_range("Illegal subtree index");
		}
	}

	NTree<T, N>* detachNTree(int aIndex)
	{
		if (isEmpty()) {
			throw std::domain_error("Empty NTree!");
		}

		if ((aIndex >= 0) && (aIndex < N)) {
			NTree<T, N>* Result = fNodes[aIndex];
			fNodes[aIndex] = &NIL;
			return Result;
		}
		else {
			throw std::out_of_range("Illegal subtree index");
		}
	}
};

template<class T, int N>
NTree<T, N> NTree<T, N>::NIL;