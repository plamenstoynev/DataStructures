#pragma once
#include <stdexcept>


template <class T>
class Queue {
private:
    T* data;
    size_t begin;
    size_t end;
    size_t size;
public:
    Queue();
    Queue(size_t size);


    void enqueue(const T& data);
    T dequeue();
    const T& first() const;

    bool isEmpty() const;
    bool isFUll() const;

    size_t count() const;
};

template<class T>
Queue<T>::Queue() {
    this->size = 16;
    this->begin = this->end = 0;
    this->data = new T[this->size + 1];
}

template<class T>
Queue<T>::Queue(size_t size) {
    this->size = size;
    this->begin = this->end = 0;
    this->data = new T[size + 1];
}

template<class T>
void Queue<T>::enqueue(const T &data) {
    if(!isFUll()) {
        this->data[this->end] = data;
        this.end = (this->end + 1) % this.size;
    }
    else
        throw std::overflow_error("Queue is full");
}

template<class T>
T Queue<T>::dequeue() {
        const T& result = first();
        this->begin = (this->begin + 1) % this->size;
        return result;
}

template<class T>
const T &Queue<T>::first() const {
    if(!isEmpty())
        return this->data[this->begin];
    else
        throw std::underflow_error("Queue is empty");
}

template<class T>
bool Queue<T>::isEmpty() const {
    return this->begin == this->end;
}

template<class T>
bool Queue<T>::isFUll() const {
    return (this->end + 1) % this->size == this->begin;
}

template<class T>
size_t Queue<T>::count() const {
    return (this->end + this->size - this->begin) % this->size;
}