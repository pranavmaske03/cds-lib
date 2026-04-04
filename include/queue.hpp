#pragma once
#include "node.hpp"
#include <iostream>
#include <stdexcept>

namespace cds {
    template <typename T>
    class queue {
    public:
        queue();
        queue(const queue& other);
        queue& operator = (const queue& other);
        ~queue();

        void enqueue(const T& val);
        T dequeue();

        const T& front() const;
        const T& back() const;

        bool empty() const noexcept;
        int size() const noexcept;
        void print() const;

    private:
        node_s<T>* head;
        node_s<T>* tail;
        int count;

        void clear();
        void copy_from(const queue& other);
    };

    template <typename T>
    queue<T>::queue() : head(nullptr), tail(nullptr), count(0) {}

    template <typename T>
    void queue<T>::clear() {
        while (head) {
            node_s<T>* t = head;
            head = head->next;
            delete t;
        }
        head = tail = nullptr;
        count = 0;
    }

    template <typename T>
    void queue<T>::copy_from(const queue& other) {
        for (node_s<T>* cur = other.head; cur != nullptr; cur = cur->next) {
            enqueue(cur->data);
        }
    }

    template <typename T>
    queue<T>::queue(const queue& other) : head(nullptr), tail(nullptr), count(0) {
        copy_from(other);
    }

    template <typename T>
    queue<T>& queue<T>::operator = (const queue& other) {
        if (this != &other) {
            clear();
            copy_from(other);
        }
        return *this;
    }

    template <typename T>
    queue<T>::~queue() {
        clear();
    }

    template <typename T>
    void queue<T>::enqueue(const T& val) {
        node_s<T>* new_node = new node_s<T>(val);

        if (tail == nullptr) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
        ++count;
    }

    template <typename T>
    T queue<T>::dequeue() {
        if (head == nullptr) 
            throw std::underflow_error("dequeue: queue is empty");

        T val = head->data;
        node_s<T>* tmp = head;
        head = head->next;

        if (head == nullptr)    
            tail = nullptr;
        delete tmp;
        --count;
        return val;
    }

    template <typename T>
    const T& queue<T>::front() const {
        if (head == nullptr) 
            throw std::underflow_error("front: queue is empty");
        return head->data;
    }

    template <typename T>
    const T& queue<T>::back() const {
        if (tail == nullptr) 
            throw std::underflow_error("back: queue is empty");
        return tail->data;
    }

    template <typename T>
    bool queue<T>::empty() const noexcept {
        return head == nullptr;
    }

    template <typename T>
    int queue<T>::size() const noexcept {
        return count;
    }

    template <typename T>
    void queue<T>::print() const {
        std::cout << "queue (front -> back) [" << count << "] : ";
        for (node_s<T>* t = head; t != nullptr; t = t->next)
            std::cout << "|" << t->data << "| ";
        std::cout << "\n";
    }
} 