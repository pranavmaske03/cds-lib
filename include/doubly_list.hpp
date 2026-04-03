#pragma once
#include "node.hpp"
#include <iostream>
#include <stdexcept>

namespace cds {
    template <typename T>
    class doubly_list {
    public:
        doubly_list();
        doubly_list(const doubly_list& other);
        doubly_list& operator = (const doubly_list& other);
        ~doubly_list();

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
        int count;

        void copy_from(const doubly_list& other);
        void clear();
    };

    template <typename T>
    doubly_list<T>::doubly_list() : head(nullptr), count(0) {}

    template <typename T>
    void doubly_list<T>::clear() {
        while (head != nullptr) {
            node_d<T>* tmp = head;
            head = head->next;
            delete tmp;
        }
        count = 0;
    }

    template <typename T>
    void doubly_list<T>::copy_from(const doubly_list& other) {
        node_d<T>* curr = other.head;
        while (curr != nullptr) {
            push_back(curr->data);
            curr = curr->next;
        }
    }

    template <typename T>
    doubly_list<T>::doubly_list(const doubly_list& other) : head(nullptr), count(0) {
        copy_from(other);
    }

    template <typename T>
    doubly_list<T>& doubly_list<T>::operator = (const doubly_list& other) {
        if (this != &other) {
            clear();
            copy_from(other);
        }
        return *this;
    }

    template <typename T>
    doubly_list<T>::~doubly_list() { 
        clear(); 
    }

    template <typename T>
    void doubly_list<T>::print() const {
        std::cout << "doubly_list [" << count <<"] : ";
        node_d<T>* tmp = head;
        while (tmp != nullptr) {
            std::cout << "|" << tmp->data << "| <=> ";
            tmp = tmp->next;
        }
        std::cout << "NULL\n";
    }

    template <typename T>
    void doubly_list<T>::push_front(const T& val) {
        node_d<T>* new_node = new node_d<T>(val);
        if (head != nullptr) {
            head->prev = new_node;
        }
        new_node->next = head;
        head = new_node;
        ++count;
    }

    template <typename T>
    void doubly_list<T>::push_back(const T& val) {
        node_d<T>* new_node = new node_d<T>(val);
        if (head == nullptr) {
            head = new_node;
        } else {
            node_d<T>* tmp = head;
            while (tmp->next != nullptr) 
                tmp = tmp->next;
            tmp->next = new_node;
            new_node->prev = tmp;
        }
        ++count;
    }

    template <typename T>
    void doubly_list<T>::insert_at(const T& val, int pos) {
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
        if (tmp->next) tmp->next->prev = new_node;
        tmp->next = new_node;
        ++count;
    }

    template <typename T>
    void doubly_list<T>::pop_front() {
        if (head == nullptr) 
            return;
        node_d<T>* tmp = head;
        head = head->next;
        if (head) head->prev = nullptr;
        delete tmp;
        --count;
    }

    template <typename T>
    void doubly_list<T>::pop_back() {
        if (head == nullptr) 
            return;
        if (head->next == nullptr) {
            delete head;
            head = nullptr;
            --count;
            return;
        }

        node_d<T>* tmp = head;
        while (tmp->next != nullptr) tmp = tmp->next;
        tmp->prev->next = nullptr;
        delete tmp;
        --count;
    }

    template <typename T>
    void doubly_list<T>::erase_at(int pos) {
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
        for (int i = 1; i < pos - 1; ++i) 
            tmp = tmp->next;

        node_d<T>* del = tmp->next;
        tmp->next = del->next;
        if (del->next) del->next->prev = tmp;
        delete del;
        --count;
    }

    template <typename T>
    int doubly_list<T>::find_first(const T& val) const {
        node_d<T>* tmp = head;
        for (int i = 1; tmp != nullptr; ++i, tmp = tmp->next)
            if (tmp->data == val) 
                return i;
        return -1;
    }

    template <typename T>
    int doubly_list<T>::find_last(const T& val) const {
        node_d<T>* tmp = head;
        int last = -1;
        for (int i = 1; tmp != nullptr; ++i, tmp = tmp->next)
            if (tmp->data == val) 
                last = i;
        return last;
    }

    template <typename T>
    int doubly_list<T>::count_of(const T& val) const {
        int cnt = 0;
        for (node_d<T>* t = head; t != nullptr; t = t->next)
            if (t->data == val) 
                ++cnt;
        return cnt;
    }

    template <typename T>
    T doubly_list<T>::min() const {
        if (head == nullptr) 
            throw std::underflow_error("min: list is empty");
        T mn = head->data;
        for (node_d<T>* t = head->next; t != nullptr; t = t->next)
            if (t->data < mn) 
                mn = t->data;
        return mn;
    }

    template <typename T>
    T doubly_list<T>::max() const {
        if (head == nullptr) 
            throw std::underflow_error("max: list is empty");
        T mx = head->data;
        for (node_d<T>* t = head->next; t != nullptr; t = t->next)
            if (t->data > mx) 
                mx = t->data;
        return mx;
    }

    template <typename T>
    T doubly_list<T>::sum() const {
        if (head == nullptr) 
            throw std::underflow_error("sum: list is empty");
        T sum = T();
        for (node_d<T>* t = head; t != nullptr; t = t->next)
            sum = sum + t->data;
        return sum;
    }

    template <typename T>
    void doubly_list<T>::reverse() {
        node_d<T>* curr = head;
        node_d<T>* last = nullptr;
        while (curr != nullptr) {
            std::swap(curr->next, curr->prev);
            last = curr;
            curr  = curr->prev; 
        }
        head = last;
    }

    template <typename T>
    bool doubly_list<T>::is_sorted() const {
        if (head == nullptr || head->next == nullptr) 
            return true;
        for (node_d<T>* t = head; t->next != nullptr; t = t->next)
            if (t->data > t->next->data) 
                return false;
        return true;
    }

    template <typename T>
    void doubly_list<T>::remove_duplicates() {
        for (node_d<T>* cur = head; cur != nullptr; cur = cur->next) {
            node_d<T>* runner = cur->next;
            while (runner != nullptr)
            {
                node_d<T>* nxt = runner->next;
                if (runner->data == cur->data)
                {
                    if (runner->prev) runner->prev->next = runner->next;
                    if (runner->next) runner->next->prev = runner->prev;
                    delete runner;
                    --count;
                }
                runner = nxt;
            }
        }
    }

    template <typename T>
    T& doubly_list<T>::operator[](int index) {
        if (index < 0 || index >= count)
            throw std::out_of_range("operator[]: index " + std::to_string(index) + " out of range");
        node_d<T>* tmp = head;
        for (int i = 0; i < index; ++i) 
            tmp = tmp->next;
        return tmp->data;
    }

    template <typename T>
    const T& doubly_list<T>::operator[](int index) const {
        if (index < 0 || index >= count)
            throw std::out_of_range("operator[]: index " + std::to_string(index) + " out of range");
        node_d<T>* tmp = head;
        for (int i = 0; i < index; ++i) 
            tmp = tmp->next;
        return tmp->data;
    }
}