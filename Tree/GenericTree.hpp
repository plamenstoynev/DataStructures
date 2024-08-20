#pragma once
#include <iostream>

template <class T>
class GenericTree {
private:
    struct Node{
        T data;
        Node* child;
        Node* sibling;

        Node(const T& data) : data(data), child(nullptr), sibling(nullptr){}
        Node(const T& data, Node* child, Node* sibling) : data(data), child(child), sibling(sibling){}
    };

    Node* root;
    size_t size;

public:
    GenericTree();

    GenericTree(const GenericTree& other);
    GenericTree(GenericTree&& other);

    GenericTree& operator=(const GenericTree& other);
    GenericTree& operator=(GenericTree&& other);

    bool find(const T& key) const;
    bool insert(const T& key, int path[]);
    bool remove(const T& key);

    size_t getSize() const;
    size_t getHigh() const;

    template <class Function>
    void map(const Function& fun);

    void print() const;

    ~GenericTree();
private:
    void free(Node* root);
    Node* copyFrom(Node* root);
    void moveFrom(GenericTree&& other);

    bool find(const Node* root, const T& key) const;
    size_t Height(const Node* root);
    bool remove(Node*& root, const T& data);
    bool insert(Node*& root, const T& key, const int* path);

    template<class Function>
    void map(const Function& func, Node* root);

};

template<class T>
GenericTree<T>::GenericTree() {
    this->root = nullptr;
    this->size = 0;
}

template<class T>
GenericTree<T>::GenericTree(const GenericTree &other) {
    root = copyFrom(other.root);
    size = other.size;
}

template<class T>
GenericTree<T>::GenericTree(GenericTree &&other) noexcept {
    moveFrom(std::move(other));
}

template<class T>
GenericTree<T> & GenericTree<T>::operator=(const GenericTree &other) {
    if(this != &other) {
        free();
        copyFrom(other);
    }
    return *this;
}

template<class T>
GenericTree<T> & GenericTree<T>::operator=(GenericTree &&other) noexcept {
    if(this != &other) {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

template<class T>
bool GenericTree<T>::find(const T &key) const {
    find(root, key);
}

template<class T>
bool GenericTree<T>::insert(const T &key, int path[]) {
    if(insert(root, key, path)) {
        size++;
        return true;
    }
    return false;
}

template<class T>
bool GenericTree<T>::remove(const T &key) {
    return remove(root,key);
}

template<class T>
size_t GenericTree<T>::getSize() const {
    return size;
}

template<class T>
size_t GenericTree<T>::getHigh() const {
    return Height(root);
}

template<class T>
template<class Function>
void GenericTree<T>::map(const Function &fun) {
    map(fun, root);
}

template<class T>
GenericTree<T>::~GenericTree() {
    free(root);
}


template<class T>
void GenericTree<T>::free(Node* root) {
    if(root) {
        free(root->child);
        free(root->sibling);
        delete root;
    }
}

template<class T>
Node* GenericTree<T>::copyFrom(Node* root) {
    return root ? new Node(root->data, copyFrom(root->child), copyFrom(root->sibling)) : root;
}
template<class T>
void GenericTree<T>::moveFrom(GenericTree &&other) {
    std::swap(root, other.root);
    std::swap(size, other.size);
}

template<class T>
bool GenericTree<T>::find(const Node *root, const T &key) const {
    if(!root)
        return false;
    if(root->data == key)
        return true;
    return find(root->sibling, key) || find(root->child, key);

}

template<class T>
size_t GenericTree<T>::Height(const Node *root) {
    if(!root)
        return 0;
    return std::max(1+Height(root->child), Height(root->sibling));
}

template<class T>
bool GenericTree<T>::remove(Node *&root, const T &data) {
    if(!root)
        return false;

    if(root->data == data) {
        Node* temp = root;
        if(!root->child)
            root = root->sibling;
        else if(root->sibling) {
            Node* it = root->child;
            while(it->sibling) {
                it = it->sibling;
                it->sibling = it->sibling->child;
                root->sibling->child = root->child;
                root = root->sibling;
            }
        }
        else {
            root = root->child;
        }
        --size;
        delete temp;
        return true;
    }
    else
        return remove(root->sibling, data) || remove(root->child, data);
}

template<class T>
bool GenericTree<T>::insert(Node *&root, const T &key, const int *path) {
    if(path[0] < 0) {
        root = new Node(key, nullptr, root);
        return true;
    }

    if(!root)
        return false;

    if(path[0] == 0)
        return insert(root->child, key, path+1);

    Node* it = root;
    int position = path[0];
    while(it->sibling && position > 0) {
        it = it->sibling;
        --position;
    }
    return it->sibling ? insert(it->sibling->child, key,path+1) : false;
}

template<class T>
template<class Function>
void GenericTree<T>::map(const Function &func, Node *root) {
    if(root) {
        func(root->data);
        map(func, root->sibling);
        map(func, root->child);
    }
}

template<class T>
void GenericTree<T>::print() const {
    map([](const T& data) {std::cout << data << ' ';});
    std::cout << std::endl;
}