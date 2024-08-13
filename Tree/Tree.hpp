#pragma once
#include <__bit_reference>

#include "../Queue/Queue.hpp"

template <class T>

class BST {
private:
    struct Node {
        T key;
        Node* left;
        Node* right;
        Node() = default;
        Node(const T& key) : key(key), left(nullptr), right(nullptr){}
        Node(const T& key, Node* right, Node* left) : key(key), right(right), left(left){}
    };

    Node* root;
    size_t size;

public:
    BST() : root(nullptr), size(0){}

    BST(const BST& otjer);
    BST(BST&& other);

    BST& operator=(const BST& other);
    BST& operator=(BST&& other) noexcept;

    bool find(const T& key) const;
    void insert(const T& key);
    void reomve(const T& key);

    size_t getSize() const;

    Node* extractMin(Node*& root); // defenition and this function is not make by me

    ~BST(){free(root);}

private:
    bool find(const Node* root, const T& key) const;
    void insert(Node*& root, const T& key);
    void remove(Node* root, const T& key);

    void free(Node* root);
    Node* copyFrom(const BST& other);
    void moveFrom(BST&& other);
};

template<class T>
BST<T> & BST<T>::operator=(const BST &other) {
    if(this != &other) {
        free(root);
        this->root = copyFrom(other);
        this->size = other.size;
    }
    return *this;
}

template<class T>
BST<T> & BST<T>::operator=(BST &&other) noexcept {
    if(this != &other) {
        free(root);
        moveFrom(std::move(other));
    }
    return *this;
}

template<class T>
bool BST<T>::find(const T &key) const{
    find(this->root, key);
}

template<class T>
void BST<T>::insert(const T &key) {
    insert(this->root, key);
    this->size++;
}

template<class T>
void BST<T>::reomve(const T &key) {
    remove(this->root, key);
    this->size--;
}

template<class T>
size_t BST<T>::getSize() const {
    return this->size;
}

template<class T>
typename BST<T>::Node * BST<T>::extractMin(Node *&root) {
    if (root->left) {
        return extractMin(root->left);
    }

    node* n = root;
    root = root->right;
    return n;
}

template<class T>
bool BST<T>::find(const Node *root, const T &key) const {
    if(!root)
        return false;

    if(root->key == key)
        return true;
    else
        key < root->key ? find(root->left, key) : find(root->right, key);
}

template<class T>
void BST<T>::insert(Node *&root, const T &key) {
    if(!root)
        root = new Node(key);
    else
        insert(key < root->key ? root->left, root->right, key);
}

template<class T>
void BST<T>::remove(Node *root, const T &key) {
    if(!root)
        return;

    if(root->key == key) {
        Node* temp = root;

        if(!root->left)
            root = root->right;
        else if(!root->right)
            root = root->left;
        else {
            Node* mR = extractMin(root->right);
            mR->left = root->left;
            mR->right = root->right;
            root = mR;
        }
        this->size--;
        delete temp;
    }
    else
        remove(key < root->key ? root->left : root->right, key);
}

template<class T>
void BST<T>::free(Node *root) {
    if(this->root) {
        free(root->left);
        free(root->right);
        delete root;
    }
}

template<class T>
Node* BST<T>::copyFrom(const BST &other) {
    return root ?
        new Node(root->key, copyFrom(root->left), copyFrom(root->right)) : root;
}

template<class T>
void BST<T>::moveFrom(BST &&other) {
    std::swap(root, other.root);
    std::swap(size, other.size);
}