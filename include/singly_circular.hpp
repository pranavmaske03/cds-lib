#pragma once
#include "node.hpp"
#include <iostream>
#include <stdexcept>

namespace cds {
    template <typename T>
    class singly_circular {
    public:
        singly_circular();
        singly_circular(const singly_circular& other);
        singly_circular& operator = (const singly_circular& other);
        ~singly_circular();

        void print() const;
        int size() const noexcept { return count; }
        bool empty() const noexcept { return head == nullptr; }

        void push_front(const T& val);
        void push_back(const T& val);
        void insert_at(const T& val, int pos);

        void pop_front();
        void pop_back();
        void erase_at(int pos);

        int find_first(const T& val) const;
        int find_last(const T& val) const;
        int count_of(const T& val) const;
        T min() const;
        T max() const;
        T sum() const;

        void reverse();
        bool is_sorted() const;
        void remove_duplicates();

        T& operator[](int index);
        const T& operator[](int index) const;

    private:
        node_s<T>* head;
        node_s<T>* tail;
        int count;

        void copy_from(const singly_circular& other);
        void clear();
    };

    template <typename T>
    singly_circular<T>::singly_circular() : head(nullptr), tail(nullptr), count(0) {}

    template <typename T>
    void singly_circular<T>::clear() {
        if (head == nullptr) return;
        tail->next = nullptr;
        node_s<T>* tmp = head;
        while (tmp != nullptr) {
            node_s<T>* del = tmp;
            tmp = tmp->next;
            delete del;
        }
        head = tail = nullptr;
        count = 0;
    }

    template <typename T>
    void singly_circular<T>::copy_from(const singly_circular& other) {
        node_s<T>* curr = other.head;
        for (int i = 0; i < other.count; ++i) {
            push_back(curr->data);
            curr = curr->next;
        }
    }

    template <typename T>
    singly_circular<T>::singly_circular(const singly_circular& other) : head(nullptr), tail(nullptr), count(0) {
        copy_from(other);
    }

    template <typename T>
    singly_circular<T>& singly_circular<T>::operator = (const singly_circular& other) {
        if (this != &other) {
            clear();
            copy_from(other);
        }
        return *this;
    }

    template <typename T>
    singly_circular<T>::~singly_circular() { 
        clear(); 
    }

    template <typename T>
    void singly_circular<T>::print() const {
        if (head == nullptr) { std::cout << "singly_circular [empty]\n"; return; } 
        std::cout << "singly_circular [" << count <<"] : ";
        node_s<T>* tmp = head;
        do {
            std::cout << "|" << tmp->data << "|-> ";
            tmp = tmp->next;
        } while (tmp != tail->next);
        std::cout << "(back to head)\n";
    }

    template <typename T>
    void singly_circular<T>::push_front(const T& val) {
        node_s<T>* new_node = new node_s<T>(val);
        if (head == nullptr) {
            head = tail = new_node;
        } else {
            new_node->next = head;
            head = new_node;
        }
        tail->next = head;
        ++count;
    }

    template <typename T>
    void singly_circular<T>::push_back(const T& val) {
        node_s<T>* new_node = new node_s<T>(val);
        if (head == nullptr) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
        tail->next = head;
        ++count;
    }

    template <typename T>
    void singly_circular<T>::insert_at(const T& val, int pos) {
        if (pos < 1 || pos > count + 1)
            throw std::out_of_range("insert_at: invalid position " + std::to_string(pos));

        if (pos == 1) { 
            push_front(val); 
            return; 
        }
        if (pos == count + 1) { 
            push_back(val); 
            return; 
        }

        node_s<T>* tmp = head;
        for (int i = 1; i < pos - 1; i++) 
            tmp = tmp->next;

        node_s<T>* new_node = new node_s<T>(val);
        new_node->next = tmp->next;
        tmp->next = new_node;
        ++count;
    }

    template <typename T>
    void singly_circular<T>::pop_front() {
        if (head == nullptr) 
            return;
        if (head == tail) {
            delete head;
            head = tail = nullptr;
            --count;
            return;
        }
        node_s<T>* tmp = head;
        head = head->next;
        tail->next = head;
        delete tmp;
        --count;
    }

    template <typename T>
    void singly_circular<T>::pop_back() {
        if (head == nullptr) 
            return;
        if(head == tail) {
            delete head;
            head = tail = nullptr;
            --count;
            return;
        }

        node_s<T>* tmp = head;
        while (tmp->next != tail) 
            tmp = tmp->next;
        delete tmp->next;
        tail = tmp;
        tail->next = head;
        --count;
    }

    template <typename T>
    void singly_circular<T>::erase_at(int pos) {
        if (pos < 1 || pos > count)
            throw std::out_of_range("erase_at: invalid position " + std::to_string(pos));

        if (pos == 1) {
            pop_front();
            return;
        }
        if (pos == count) {
            pop_back();
            return;
        }

        node_s<T>* tmp = head;
        for (int i = 1; i < pos - 1; ++i) 
            tmp = tmp->next;

        node_s<T>* del = tmp->next;
        tmp->next = del->next;
        delete del;
        --count;
    }

    template <typename T>
    int singly_circular<T>::find_first(const T& val) const {
        node_s<T>* tmp = head;
        for (int i = 1; i <= count; ++i, tmp = tmp->next)
            if (tmp->data == val) 
                return i;
        return -1;
    }

    template <typename T>
    int singly_circular<T>::find_last(const T& val) const {
        node_s<T>* tmp = head;
        int last = -1;
        for (int i = 1; i <= count; ++i, tmp = tmp->next)
            if (tmp->data == val) 
                last = i;
        return last;
    }

    template <typename T>
    int singly_circular<T>::count_of(const T& val) const {
        int cnt = 0;
        node_s<T>* t = head;
        for (int i = 0; i < count; ++i, t = t->next)
            if (t->data == val) ++cnt;
        return cnt;
    }

    template <typename T>
    T singly_circular<T>::min() const {
        if (head == nullptr) 
            throw std::underflow_error("min: list is empty");
        T mn = head->data;
        node_s<T>* t = head->next;
        for (int i = 1; i < count; ++i, t = t->next)
            if (t->data < mn) mn = t->data;
        return mn;
    }

    template <typename T>
    T singly_circular<T>::max() const {
        if (head == nullptr) 
            throw std::underflow_error("max: list is empty");
        T mx = head->data;
        node_s<T>* t = head->next;
        for (int i = 1; i < count; ++i, t = t->next)
            if (t->data > mx) mx = t->data;
        return mx;
    }

    template <typename T>
    T singly_circular<T>::sum() const {
        if (head == nullptr) 
            throw std::underflow_error("sum: list is empty");
        T sum = T();
        node_s<T>* t = head;
        for (int i = 0; i < count; ++i, t = t->next)
            sum += t->data;
        return sum;
    }

    template <typename T>
    void singly_circular<T>::reverse() {
        if (head == nullptr || head == tail) return;

        node_s<T>* prev = nullptr;
        node_s<T>* curr = head;

        for (int i = 0; i < count; ++i) {
            node_s<T>* next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }

        std::swap(head, tail);
        tail->next = head;
    }

    template <typename T>
    bool singly_circular<T>::is_sorted() const {
        if (head == nullptr || head->next == nullptr) 
            return true;
        node_s<T>* t = head;
        for (int i = 0; i < count - 1; ++i, t = t->next)
            if (t->data > t->next->data) return false;
        return true;
    }

    template <typename T>
    void singly_circular<T>::remove_duplicates() {
        if(head == nullptr || head == tail) return;
        node_s<T>* cur = head;
        for (int i = 0; i < count; ++i, cur = cur->next) {
            node_s<T>* runner = cur;
            int inner_steps = count - i - 1;
            for (int j = 0; j < inner_steps; ++j) {
                if (runner->next->data == cur->data) {
                    node_s<T>* dup = runner->next;
                    runner->next = dup->next;
                    if (dup == tail) tail = runner;
                    delete dup;
                    --count;
                } else {
                    runner = runner->next;
                }
            }
        }
    }

    template <typename T>
    T& singly_circular<T>::operator[](int index) {
        if (index < 0 || index >= count)
            throw std::out_of_range("operator[]: index " + std::to_string(index) + " out of range");
        node_s<T>* temp = head;
        for (int i = 0; i < index; ++i) 
            temp = temp->next;
        return temp->data;
    }

    template <typename T>
    const T& singly_circular<T>::operator[](int index) const {
        if (index < 0 || index >= count)
            throw std::out_of_range("operator[]: index " + std::to_string(index) + " out of range");
        node_s<T>* temp = head;
        for (int i = 0; i < index; ++i) 
            temp = temp->next;
        return temp->data;
    }
}