#pragma once
#include "node.hpp"
#include <iostream>
#include <stdexcept>

namespace cds {
    template <typename T>
    class singly_list {
    public:
        singly_list();
        singly_list(const singly_list& other);
        singly_list& operator = (const singly_list& other);
        ~singly_list();

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
        int count;

        void copy_from(const singly_list& other);
        void clear();
    };

    template <typename T>
    singly_list<T>::singly_list() : head(nullptr), count(0) {}

    template <typename T>
    void singly_list<T>::clear() {
        while (head != nullptr) {
            node_s<T>* temp = head;
            head = head->next;
            delete temp;
        }
        count = 0;
    }

    template <typename T>
    void singly_list<T>::copy_from(const singly_list& other) {
        node_s<T>* curr = other.head;
        while (curr != nullptr) {
            push_back(curr->data);
            curr = curr->next;
        }
    }

    template <typename T>
    singly_list<T>::singly_list(const singly_list& other) : head(nullptr), count(0) {
        copy_from(other);
    }

    template <typename T>
    singly_list<T>& singly_list<T>::operator = (const singly_list& other) {
        if (this != &other) {
            clear();
            copy_from(other);
        }
        return *this;
    }

    template <typename T>
    singly_list<T>::~singly_list() { 
        clear(); 
    }

    template <typename T>
    void singly_list<T>::print() const {
        std::cout << "singly_list [" << count <<"] : ";
        node_s<T>* tmp = head;
        while (tmp != nullptr) {
            std::cout << "|" << tmp->data << "|-> ";
            tmp = tmp->next;
        }
        std::cout << "NULL\n";
    }

    template <typename T>
    void singly_list<T>::push_front(const T& val) {
        node_s<T>* new_node = new node_s<T>(val);
        new_node->next = head;
        head = new_node;
        ++count;
    }

    template <typename T>
    void singly_list<T>::push_back(const T& val) {
        node_s<T>* new_node = new node_s<T>(val);
        if (head == nullptr) {
            head = new_node;
        } else {
            node_s<T>* tmp = head;
            while (tmp->next != nullptr) 
                tmp = tmp->next;
            tmp->next = new_node;
        }
        ++count;
    }

    template <typename T>
    void singly_list<T>::insert_at(const T& val, int pos) {
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
    void singly_list<T>::pop_front() {
        if (head == nullptr) 
            return;
        node_s<T>* temp = head;
        head = head->next;
        delete temp;
        --count;
    }

    template <typename T>
    void singly_list<T>::pop_back() {
        if (head == nullptr) 
            return;
        if (head->next == nullptr) {
            delete head;
            head = nullptr;
            --count;
            return;
        }

        node_s<T>* temp = head;
        while (temp->next->next != nullptr) 
            temp = temp->next;
        delete temp->next;
        temp->next = nullptr;
        --count;
    }

    template <typename T>
    void singly_list<T>::erase_at(int pos) {
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

        node_s<T>* temp = head;
        for (int i = 1; i < pos - 1; ++i) 
            temp = temp->next;

        node_s<T>* del = temp->next;
        temp->next = del->next;
        delete del;
        --count;
    }

    template <typename T>
    int singly_list<T>::find_first(const T& val) const {
        node_s<T>* temp = head;
        for (int i = 1; temp != nullptr; ++i, temp = temp->next)
            if (temp->data == val) 
                return i;
        return -1;
    }

    template <typename T>
    int singly_list<T>::find_last(const T& val) const {
        node_s<T>* temp = head;
        int last = -1;
        for (int i = 1; temp != nullptr; ++i, temp = temp->next)
            if (temp->data == val) 
                last = i;
        return last;
    }

    template <typename T>
    int singly_list<T>::count_of(const T& val) const {
        int cnt = 0;
        for (node_s<T>* t = head; t != nullptr; t = t->next)
            if (t->data == val) 
                ++cnt;
        return cnt;
    }

    template <typename T>
    T singly_list<T>::min() const {
        if (head == nullptr) 
            throw std::underflow_error("min: list is empty");
        T mn = head->data;
        for (node_s<T>* t = head->next; t != nullptr; t = t->next)
            if (t->data < mn) 
                mn = t->data;
        return mn;
    }

    template <typename T>
    T singly_list<T>::max() const {
        if (head == nullptr) 
            throw std::underflow_error("max: list is empty");
        T mx = head->data;
        for (node_s<T>* t = head->next; t != nullptr; t = t->next)
            if (t->data > mx) 
                mx = t->data;
        return mx;
    }

    template <typename T>
    T singly_list<T>::sum() const {
        if (head == nullptr) 
            throw std::underflow_error("sum: list is empty");
        T sum = T();
        for (node_s<T>* t = head; t != nullptr; t = t->next)
            sum = sum + t->data;
        return sum;
    }

    template <typename T>
    void singly_list<T>::reverse() {
        node_s<T>* prev = nullptr;
        node_s<T>* curr = head;
        while (curr != nullptr) {
            node_s<T>* next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        head = prev;
    }

    template <typename T>
    bool singly_list<T>::is_sorted() const {
        if (head == nullptr || head->next == nullptr) 
            return true;
        for (node_s<T>* t = head; t->next != nullptr; t = t->next)
            if (t->data > t->next->data) 
                return false;
        return true;
    }

    template <typename T>
    void singly_list<T>::remove_duplicates() {
        for (node_s<T>* cur = head; cur != nullptr; cur = cur->next) {
            node_s<T>* runner = cur;
            while (runner->next != nullptr) {
                if (runner->next->data == cur->data) {
                    node_s<T>* dup = runner->next;
                    runner->next = dup->next;
                    delete dup;
                    --count;
                } else {
                    runner = runner->next;
                }
            }
        }
    }

    template <typename T>
    T& singly_list<T>::operator[](int index) {
        if (index < 0 || index >= count)
            throw std::out_of_range("operator[]: index " + std::to_string(index) + " out of range");
        node_s<T>* temp = head;
        for (int i = 0; i < index; ++i) 
            temp = temp->next;
        return temp->data;
    }

    template <typename T>
    const T& singly_list<T>::operator[](int index) const {
        if (index < 0 || index >= count)
            throw std::out_of_range("operator[]: index " + std::to_string(index) + " out of range");
        node_s<T>* temp = head;
        for (int i = 0; i < index; ++i) 
            temp = temp->next;
        return temp->data;
    }
}