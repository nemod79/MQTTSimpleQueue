#ifndef SIMPLE_QUEUE_H_
#define SIMPLE_QUEUE_H_

#include <vector>
#include <iostream>
#include <mutex>
#include <condition_variable>   

// We are providing a "circular buffer", in order to limit the memory used we are limiting the elements that will be rotated through
#define MAX_ELEMENTS 1000

template <class T>

class SimpleQueue{
    private:
        std::vector<T> *_queue;   // Pointer to the object "queue"
        std::mutex _mtx;

    public:
        SimpleQueue();                  // Constructor, used for allocating all the resources needed
        ~SimpleQueue();
        T GetElement();                  // Retrieve one queue element, the oldest one
        void PushElement(T &&element);   // Retrieve here the oldest element in queue
        bool IsEmpty();                   
        unsigned long GetQueueSize();
};

template <typename T>
inline SimpleQueue<T>::SimpleQueue() {
    _queue = new std::vector<T>;
}

template <typename T>
inline SimpleQueue<T>::~SimpleQueue() {
    delete _queue;
}

template <typename T>
inline void SimpleQueue<T>::PushElement(T &&element) {
    std::lock_guard<std::mutex> LockMTX(_mtx);

    if (_queue->size() >= MAX_ELEMENTS)     // let's remove the oldest element if exceeding MAX_ELEMENTS, working like a circular buffer
        _queue->erase(_queue->begin());
    
    _queue->push_back(element);
}

template <typename T>
inline T SimpleQueue<T>::GetElement() {
    std::lock_guard<std::mutex> LockMTX(_mtx);
    T element = _queue->front();            // implementing here a FIFO
    _queue->erase(_queue->begin());
    return element;
}

template <typename T>
inline bool SimpleQueue<T>::IsEmpty() { 
    if (_queue->size() != 0)
        return false;
    else
        return true;
}

template <typename T>
inline unsigned long SimpleQueue<T>::GetQueueSize() {
    std::lock_guard<std::mutex> LockMTX(_mtx);
    return _queue->size();
}

#endif