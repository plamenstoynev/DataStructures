#pragma once
#include <iostream>
template<class T>
class LinkedList{
private:
    struct Node{
        T data;
        Node* next;
        Node(const T& data, Node* next = nullptr);
    };
    Node* head = nullptr;
    Node* tail = nullptr;
    size_t size = 0;
public:
    LinkedList() = default;
    LinkedList(const std::initializer_list<T>& values);
    LinkedList(const T* arr, size_t size);
    LinkedList(const LinkedList& other);
    LinkedList& operator=(const LinkedList& other);
    LinkedList(LinkedList&& other);
    LinkedList& operator=(LinkedList&& other);

    const T& front() const;
    const T& back() const;
    const T& operator[](size_t index) const;

    class iterator;

    iterator begin();
    iterator end();

    bool isEmpty() const;
    size_t getSize() const;

    void pushFront(const T& data);
    void pushEnd(const T& data);
    void pushAtIndex(size_t index, const T& data);

    void popFront();
    void popEnd();
    T popAtIndex(size_t index);
    void popAfter(iterator iter);

    void print() const;

    ~LinkedList();
private:
    const T& atPosition(size_t index) const;
    const T& popIndex(size_t index);
    void copyFrom(const LinkedList& other);
    void moveFrom(LinkedList&& other);
    void free();
};

template<class T>
class LinkedList<T>::iterator{
private:
    friend class LinkedList<T>;
    LinkedList<T>::Node* ptr;
    const LinkedList<T> owner;

    iterator(LinkedList<T>::Node* ptr, const LinkedList<T>& creator) : ptr(ptr), owner(creator){}
public:
    bool valid() const{
        return this->ptr;
    }
    operator bool() const{
        return this->ptr;
    }

    T& operator*(){
        return this->ptr->data;
    }
    T* operator->(){
        return &this->ptr->data;
    }

    iterator& operator++(){
        this->ptr = this->ptr->next;
        return *this;
    }
    iterator operator++(int){
        iterator res = *this;
        ++*this;
        return res;
    }

    bool operator==(const iterator& rhs) const{
        return this->ptr == rhs->ptr;
    }

    bool operator!=(const iterator& rhs) const{
        return !(*this == rhs);
    }
};

template<class T>
LinkedList<T>::Node::Node(const T& data, Node* next) : data(data), next(next) {}

template <class T>
LinkedList<T>::LinkedList(const std::initializer_list<T> &values) {
    for(const T& value : values)
        pushEnd(value);
}
template<class T>
LinkedList<T>::LinkedList(const T *arr, size_t size) {
    this->size = size;
    for(size_t i = 0; i < size; i++)
        pushEnd(arr[i]);
}
template<class T>
LinkedList<T>::LinkedList(const LinkedList<T> &other) {
    copyFrom(other);
}
template<class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T> &other) {
    if(*this != other){
        free();
        copyFrom(other);
    }
    return *this;
}
template<class T>
LinkedList<T>::LinkedList(LinkedList<T> &&other) {
    moveFrom(std::move(other));
}
template<class T>
LinkedList<T> &LinkedList<T>::operator=(LinkedList<T> &&other) {
    if(*this != other){
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

template<class T>
void LinkedList<T>::pushFront(const T &data) {
    pushAtIndex(0, data);
}

template<class T>
void LinkedList<T>::pushEnd(const T &data) {
    pushAtIndex(this->size, data);
}

template <class T>
void LinkedList<T>::pushAtIndex(size_t index, const T &data) {
    if(index > this->size)
        throw std::exception();

    if(index == 0) {
        this->head = new Node(data, this->head);
        if(!this->size)
           this->tail = this->head;
    }
    else if(index == this->size){
        this->tail->next = new Node(data);
        tail = tail->next;
    }
    else{
        Node* iter = head;
        for(size_t i = 0; i < index - 1; i++){
            iter = iter->next;
        }
        iter->next = new Node(data, iter->next);
    }
    this->size++;
}

template<class T>
bool LinkedList<T>::isEmpty() const {
    return this->size == 0;
}
template<class T>
size_t LinkedList<T>::getSize() const {
    return this->size;
}

template<class T>
typename LinkedList<T>::iterator LinkedList<T>::begin() {
    return iterator(this->head, *this);
}

template<class T>
typename LinkedList<T>::iterator LinkedList<T>::end() {
    return iterator(nullptr, *this);
}

template<class T>
const T &LinkedList<T>::front() const {
    return atPosition(0);
}

template<class T>
const T &LinkedList<T>::back() const {
    return atPosition(this->size - 1);
}

template <class T>
const T& LinkedList<T>::atPosition(size_t index) const {
    if(isEmpty() || index > this->size)
        throw std::exception();

    if(index == 0)
        return this->head->data;

    if(index == this->size - 1)
        return this->tail->data;

    const Node* iter = this->head;
    for(size_t i = 0; i < index - 1; i++)
        iter = iter->next;

    return iter->data;
}

template <class T>
void LinkedList<T>::popFront() {
    popIndex(0);
}

template <class T>
void LinkedList<T>::popEnd() {
    popIndex(this->size-1);
}

template <class T>
void LinkedList<T>::popAfter(LinkedList::iterator iter) {
    if(&iter.owner != this)
        throw std::exception();

    if(!iter.ptr)
        throw std::exception();

    if(!iter.ptr->next)
        return;

    Node* toDelete = iter.ptr->next;
    iter.ptr->next = toDelete->next;
    if(tail == nullptr)
        tail = iter.ptr;

    --this->size;
    delete toDelete;
}

template <class T>
const T &LinkedList<T>::popIndex(size_t index) {
    if(index > this->size)
        throw std::exception();


    Node* remove = this->head;

    if(index == 0){
        this->head = this->head->next;
        if(this->head == nullptr)
            this->tail = nullptr;
    }
    else{
        Node* iter = this->head;
        for(size_t i = 0; i < index - 1; i++)
            iter = iter->next;

        remove = iter->next;
        iter->next = remove->next;

        if(remove == this->tail)
            this->tail = iter;
    }
    this->size--;
    T store = remove->data;
    delete remove;
    return store;
}

template<class T>
const T &LinkedList<T>::operator[](size_t index) const {
    return atPosition(index);
}

template<class T>
T LinkedList<T>::popAtIndex(size_t index) {
    if(isEmpty() || index > this->size)
        throw std::exception();

    Node* remove = this->head;

    if(index == 0){
        head = head->next;
        if(!head)
            tail = nullptr;
    }
    else{
        Node* iter = head;
        for(size_t i = 0; i < index; i++){
            iter = iter->next;
        }
        remove = iter->next;
        iter->next = remove->next;


        if(remove == tail)
            this->tail = iter;
    }

    T store = remove->data;
    delete remove;
    return store;
}

template<class T>
void LinkedList<T>::print() const {
    Node* iter = this->head;
    while(iter){
        std::cout << iter->data << " ";
        iter = iter->next;
    }
    std::cout << std::endl;
}

template<class T>
LinkedList<T>::~LinkedList() {
    free();
}

template<class T>
void LinkedList<T>::copyFrom(const LinkedList<T> &other) {
    Node temp;
    Node* iter = &temp;
    Node* current = other.head;
    while(current){
        iter->next = new Node(current->data);
        iter = iter->next;
        current = current->next;
    }
    this->head = temp.next;
    this->tail = head ? iter : nullptr;
    this->size = other.size;
}

template<class T>
void LinkedList<T>::moveFrom(LinkedList<T> &&other) {
    std::swap(this->head, other.head);
    std::swap(this->tail, other.tail);
    this->size = other.size;
}

template<class T>
void LinkedList<T>::free() {
    Node* iter = this->head;
    Node* remove = this->head;

    while(iter){
        remove = iter;
        iter = iter->next;
        delete remove;
    }

    this->head = this->tail = nullptr;
    this->size = 0;
}