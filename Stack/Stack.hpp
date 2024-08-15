#pragma once
#include <iostream>

template <class T>
class Stack{
private:
    T* date;
    size_t topOfTheStack;
    size_t size;
public:
    Stack();
    Stack(size_t size);

    Stack(const Stack& other);
    Stack(Stack&& other);

    Stack& operator=(const Stack& other);
    Stack& operator=(Stack&& other);

    void push(T& elem);
    void pop();
    T& top() const;
    bool isEmpty() const;
    bool isFull() const;

    ~Stack();

private:
    void free();
    void copyFrom(const Stack& other);
    void moveFrom(Stack&& other);
};

template<class T>
Stack<T>::Stack() {
    this->size = 16;
    this->date = new  T[this->size];
    this->topOfTheStack = 0;
}

template<class T>
Stack<T>::Stack(size_t size) {
    this->size = size;
    this->date = new T[size];
    this->topOfTheStack = 0;
}

template<class T>
Stack<T>::Stack(const Stack &other) {
    copyFrom(other);
}

template<class T>
Stack<T>::Stack(Stack &&other) {
    moveFrom(std::move(other));
}

template<class T>
Stack<T> & Stack<T>::operator=(const Stack &other) {
    if(this != &other) {
        free()();
        copyFrom(other);
    }
}

template<class T>
Stack<T> & Stack<T>::operator=(Stack &&other) {
    if(this != & other) {
        free();
        moveFrom(std::move(other));
    }
}

template<class T>
void Stack<T>::push(T &elem) {
    if(!isFull())
        this->date[topOfTheStack] = elem;
    else
        throw std::overflow_error("Stack is full");
}

template<class T>
void Stack<T>::pop() {
    if(!isEmpty())
        topOfTheStack--;
    else
        throw std::underflow_error("Stack is empty");
}

template<class T>
T & Stack<T>::top() const {
    return this->date[this->topOfTheStack];
}

template<class T>
bool Stack<T>::isEmpty() const {
    return topOfTheStack == 0;
}

template<class T>
bool Stack<T>::isFull() const {
    return this->topOfTheStack == this->size;
}

template<class T>
Stack<T>::~Stack() {
    free();
}

template<class T>
void Stack<T>::free() {
    delete this->date;
    this->date = nullptr;
    this->size = 0;
    this->topOfTheStack = 0;
}

template<class T>
void Stack<T>::copyFrom(const Stack &other) {
    this->date = new T[other.size];
    this->size = other.size;
    this->topOfTheStack = other.topOfTheStack;
    for(size_t i = 0; i < other.size; i++)
        this->date[i] = other.date[i];
}

template<class T>
void Stack<T>::moveFrom(Stack &&other) {
    this->date = other.date;
    this->size = other.size;
    this->topOfTheStack = other.topOfTheStack;

    other.date = nullptr;
    other.size = 0;
    other.topOfTheStack = 0;
}