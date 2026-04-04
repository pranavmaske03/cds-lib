#pragma once
#include "node.hpp"
#include <iostream>
#include <stdexcept>

namespace cds {
    template <typename T>
    class stack {
    public:
        stack();
        stack(const stack& other);
        stack& operator = (const stack& other);
        ~stack();

        void push(const T& val);
        T pop();
        T top() const;

        bool empty() const noexcept;
        int  size()  const noexcept;
        void print() const;

    private:
        node_s<T>* head;
        int count;
        
        void clear();
        void copy_from(const stack& other);
    };

    template <typename T>
    stack<T>::stack() : head(nullptr), count(0) {}
    
    template <typename T>
    void stack<T>::clear() {
        while (head != nullptr) {
            node_s<T>* t = head;
            head = head->next;
            delete t;
        }
        count = 0;
    }

    template <typename T>
    void stack<T>::copy_from(const stack& other) {
        if (!other.head) return;

        T* vals = new T[other.count];
        node_s<T>* cur = other.head;
        for (int i = other.count - 1; i >= 0; --i, cur = cur->next)
            vals[i] = cur->data;
        for (int i = 0; i < other.count; ++i)
            push(vals[i]);
        delete[] vals;
    }

    template <typename T>
    stack<T>::stack(const stack& other) : head(nullptr), count(0) {
        copy_from(other);
    }

    template <typename T>
    stack<T>& stack<T>::operator = (const stack& other) {
        if (this != &other) {
            clear();
            copy_from(other);
        }
        return *this;
    }

    template <typename T>
    stack<T>::~stack() {
        clear();
    }

    template <typename T>
    void stack<T>::push(const T& val) {
        node_s<T>* n = new node_s<T>(val);
        n->next = head;
        head = n;
        ++count;
    }

    template <typename T>
    T stack<T>::pop() {
        if (head == nullptr) 
            throw std::underflow_error("pop: stack is empty");

        T val = head->data;
        node_s<T>* tmp = head;
        head = head->next;
        delete tmp;
        --count;
        return val;
    }

    template <typename T>
    T stack<T>::top() const {
        if (head == nullptr) 
            throw std::underflow_error("top: stack is empty");
        return head->data;
    }

    template <typename T>
    bool stack<T>::empty() const noexcept {
        return head == nullptr;
    }

    template <typename T>
    int stack<T>::size() const noexcept {
        return count;
    }

    template <typename T>
    void stack<T>::print() const {
        std::cout << "stack (top->bottom) [" << count << "] : ";
        for (node_s<T>* t = head; t != nullptr; t = t->next)
            std::cout << "|" << t->data << "| ";
        std::cout << "\n";
    }
}