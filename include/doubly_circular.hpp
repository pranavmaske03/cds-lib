#pragma once
#include "node.hpp"
#include <iostream>
#include <stdexcept>

namespace cds {
    template <typename T>
    class doubly_circular {
    public:
        doubly_circular();
        doubly_circular(const doubly_circular& other);
        doubly_circular& operator = (const doubly_circular& other);
        ~doubly_circular();

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
        node_d<T>* head;
        node_d<T>* tail;
        int count;

        void copy_from(const doubly_circular& other);
        void clear();
    };

    template <typename T>
    doubly_circular<T>::doubly_circular() : head(nullptr), tail(nullptr), count(0) {}

    template <typename T>
    void doubly_circular<T>::clear() {
        if (head == nullptr) return;
        head->prev = nullptr;
        tail->next = nullptr;
        node_d<T>* tmp = head;
        while (tmp != nullptr) {
            node_d<T>* del = tmp;
            tmp = tmp->next;
            delete del;
        }
        head = tail = nullptr;
        count = 0;
    }

    template <typename T>
    void doubly_circular<T>::copy_from(const doubly_circular& other) {
        if (other.head == nullptr) return;
        node_d<T>* curr = other.head;
        for (int i = 0; i < other.count; ++i) {
            push_back(curr->data);
            curr = curr->next;
        }
    }

    template <typename T>
    doubly_circular<T>::doubly_circular(const doubly_circular& other)
        : head(nullptr), tail(nullptr), count(0) {
        copy_from(other);
    }

    template <typename T>
    doubly_circular<T>& doubly_circular<T>::operator = (const doubly_circular& other) {
        if (this != &other) {
            clear();
            copy_from(other);
        }
        return *this;
    }

    template <typename T>
    doubly_circular<T>::~doubly_circular() {
        clear();
    }

    template <typename T>
    void doubly_circular<T>::print() const {
        if (head == nullptr) {
            std::cout << "doubly_circular [empty]\n";
            return;
        }
        std::cout << "doubly_circular [" << count << "] : ";
        node_d<T>* tmp = head;
        do {
            std::cout << "|" << tmp->data << "| <=> ";
            tmp = tmp->next;
        } while (tmp != head);
        std::cout << "(back to head)\n";
    }

    template <typename T>
    void doubly_circular<T>::push_front(const T& val) {
        node_d<T>* new_node = new node_d<T>(val);
        if (head == nullptr) {
            head = tail = new_node;
        } else {
            new_node->next = head;
            head->prev = new_node;
            head = new_node;
        }
        head->prev = tail;
        tail->next = head;
        ++count;
    }

    template <typename T>
    void doubly_circular<T>::push_back(const T& val) {
        node_d<T>* new_node = new node_d<T>(val);
        if (head == nullptr) {
            head = tail = new_node;
        } else {
            new_node->prev = tail;
            tail->next = new_node;
            tail = new_node;
        }
        head->prev = tail;
        tail->next = head;
        ++count;
    }

    template <typename T>
    void doubly_circular<T>::insert_at(const T& val, int pos) {
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
        node_d<T>* tmp = head;
        for (int i = 1; i < pos - 1; i++)
            tmp = tmp->next;

        node_d<T>* new_node = new node_d<T>(val);
        new_node->next = tmp->next;
        new_node->prev = tmp;
        tmp->next->prev = new_node;
        tmp->next = new_node;
        ++count;
    }

    template <typename T>
    void doubly_circular<T>::pop_front() {
        if (head == nullptr) return;

        if (head == tail) {
            delete head;
            head = tail = nullptr;
            --count;
            return;
        }
        node_d<T>* tmp = head;
        head = head->next;
        head->prev = tail;
        tail->next = head;

        delete tmp;
        --count;
    }

    template <typename T>
    void doubly_circular<T>::pop_back() {
        if (head == nullptr) return;

        if (head == tail) {
            delete head;
            head = tail = nullptr;
            --count;
            return;
        }
        node_d<T>* tmp = tail;
        tail = tail->prev;
        tail->next = head;
        head->prev = tail;

        delete tmp;
        --count;
    }

    template <typename T>
    void doubly_circular<T>::erase_at(int pos) {
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
        node_d<T>* tmp = head;
        for (int i = 1; i < pos; ++i)
            tmp = tmp->next;

        tmp->prev->next = tmp->next;
        tmp->next->prev = tmp->prev;

        delete tmp;
        --count;
    }

    template <typename T>
    int doubly_circular<T>::find_first(const T& val) const {
        node_d<T>* tmp = head;
        for (int i = 1; i <= count; ++i, tmp = tmp->next)
            if (tmp->data == val) return i;
        return -1;
    }

    template <typename T>
    int doubly_circular<T>::find_last(const T& val) const {
        node_d<T>* tmp = head;
        int last = -1;
        for (int i = 1; i <= count; ++i, tmp = tmp->next)
            if (tmp->data == val) last = i;
        return last;
    }

    template <typename T>
    int doubly_circular<T>::count_of(const T& val) const {
        int cnt = 0;
        node_d<T>* t = head;
        for (int i = 0; i < count; ++i, t = t->next)
            if (t->data == val) ++cnt;
        return cnt;
    }

    template <typename T>
    T doubly_circular<T>::min() const {
        if (head == nullptr)
            throw std::underflow_error("min: list is empty");
        T mn = head->data;
        node_d<T>* t = head->next;
        for (int i = 1; i < count; ++i, t = t->next)
            if (t->data < mn) mn = t->data;
        return mn;
    }

    template <typename T>
    T doubly_circular<T>::max() const {
        if (head == nullptr)
            throw std::underflow_error("max: list is empty");
        T mx = head->data;
        node_d<T>* t = head->next;
        for (int i = 1; i < count; ++i, t = t->next)
            if (t->data > mx) mx = t->data;
        return mx;
    }

    template <typename T>
    T doubly_circular<T>::sum() const {
        if (head == nullptr)
            throw std::underflow_error("sum: list is empty");
        T total = T();
        node_d<T>* t = head;
        for (int i = 0; i < count; ++i, t = t->next)
            total += t->data;
        return total;
    }

    template <typename T>
    void doubly_circular<T>::reverse() {
        if (head == nullptr || head == tail) return;

        node_d<T>* curr = head;
        for (int i = 0; i < count; ++i) {
            std::swap(curr->next, curr->prev);
            curr = curr->prev;
        }
        std::swap(head, tail);
        head->prev = tail;
        tail->next = head;
    }

    template <typename T>
    bool doubly_circular<T>::is_sorted() const {
        if (head == nullptr || head == tail) return true;
        node_d<T>* t = head;
        for (int i = 0; i < count - 1; ++i, t = t->next)
            if (t->data > t->next->data) return false;
        return true;
    }

    template <typename T>
    void doubly_circular<T>::remove_duplicates() {
        if (head == nullptr || head == tail) return;

        node_d<T>* cur = head;
        for (int i = 0; i < count; ++i, cur = cur->next) {
            node_d<T>* runner = cur->next;
            int steps = count - i - 1;
            for (int j = 0; j < steps; ++j) {
                node_d<T>* nxt = runner->next;
                if (runner->data == cur->data) {
                    runner->prev->next = runner->next;
                    runner->next->prev = runner->prev;
                    if (runner == tail) tail = runner->prev;
                    delete runner;
                    --count;
                    --steps;
                }
                runner = nxt;
            }
        }
        head->prev = tail;
        tail->next = head;
    }

    template <typename T>
    T& doubly_circular<T>::operator[](int index) {
        if (index < 0 || index >= count)
            throw std::out_of_range("index out of range");
        node_d<T>* temp = head;
        for (int i = 0; i < index; ++i)
            temp = temp->next;
        return temp->data;
    }

    template <typename T>
    const T& doubly_circular<T>::operator[](int index) const {
        if (index < 0 || index >= count)
            throw std::out_of_range("index out of range");
        node_d<T>* temp = head;
        for (int i = 0; i < index; ++i)
            temp = temp->next;
        return temp->data;
    }
}