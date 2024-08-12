#pragma once


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

