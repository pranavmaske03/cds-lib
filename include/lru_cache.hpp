#pragma once 
#include "node.hpp"
#include <iostream>
#include <stdexcept>
#include <unordered_map>

namespace cds {
    template <typename K, typename V>
    class lru_cache {
        public:
            explicit lru_cache(int cap);
            lru_cache(const lru_cache& other);
            lru_cache& operator = (const lru_cache& other);
            ~lru_cache();

            V get(const K& key);
            void put(const K& key, const V& val);
            bool contains(const K& key) const;
            void remove(const K& key);

            int size() const noexcept { return count; }
            bool empty() const noexcept { return count == 0; }
            int capacity() const noexcept { return cap; }
            bool is_full() const noexcept { return count == cap; }

            K peek_lru() const;
            K peek_mru() const;
            void print() const;
            void clear();
        
        private:
            int cap;
            int count;
            node_l<K, V>* head;
            node_l<K, V>* tail;
            std::unordered_map<K, node_l<K, V>*> cache_map;

            void move_to_front(node_l<K, V>* node);
            void insert_at_front(node_l<K, V>* node);
            void detach(node_l<K, V>* node);
            void evict_lru();
            void copy_from(const lru_cache& other);
    };

    template <typename K, typename V>
    lru_cache<K, V>::lru_cache(int cap) : cap(cap), count(0), head(nullptr), tail(nullptr) {
        if (cap <= 0) 
            throw std::invalid_argument("lru_cache: capacity must be greater than 0");
    }

    template <typename K, typename V>
    void lru_cache<K, V>::clear() {
        node_l<K, V>* curr = head;

        while (curr != nullptr) {
            node_l<K, V>* next = curr->next;
            delete curr;
            curr = next;
        }
        head = nullptr;
        tail = nullptr;
        count = 0;
        cache_map.clear();
    }

    template <typename K, typename V>
    lru_cache<K, V>::~lru_cache() {
        clear();
    }

    template <typename K, typename V>
    void lru_cache<K, V>::copy_from(const lru_cache& other) {
        cap = other.cap;
        node_l<K, V>* curr = other.tail;

        while (curr != nullptr) {
            put(curr->key, curr->val);
            curr = curr->prev;
        }
    }

    template <typename K, typename V>
    lru_cache<K, V>::lru_cache(const lru_cache& other) : cap(0), count(0), head(nullptr), tail(nullptr) {
        copy_from(other);
    }

    template <typename K, typename V>
    lru_cache<K, V>& lru_cache<K, V>::operator = (const lru_cache& other) {
        if (this != &other) {
            clear();
            copy_from(other);
        }
        return *this;
    }

    template <typename K, typename V>
    void lru_cache<K, V>::detach(node_l<K, V>* node) {
        if (node->prev) node->prev->next = node->next;
        else head = node->next;
        if (node->next) node->next->prev = node->prev;
        else tail = node->prev;

        node->prev = nullptr;
        node->next = nullptr;
    }

    template <typename K, typename V>
    void lru_cache<K, V>::insert_at_front(node_l<K, V>* node) {
        node->next = head;
        node->prev = nullptr;
        if (head) head->prev = node;
        else tail = node;
        head = node;
    }

    template <typename K, typename V>
    void lru_cache<K, V>::move_to_front(node_l<K, V>* node) {
        if (node == head) return;
        detach(node);
        insert_at_front(node);
    }

    template <typename K, typename V>
    void lru_cache<K, V>::evict_lru() {
        if (tail == nullptr) return;
        cache_map.erase(tail->key);
        node_l<K, V>* tmp = tail;
        detach(tmp);
        delete tmp;
        --count;
    }

    template <typename K, typename V>
    V lru_cache<K, V>::get(const K& key) {
        auto it = cache_map.find(key);
        if (it == cache_map.end()) 
            throw std::out_of_range("get: key not found");
        move_to_front(it->second);
        return it->second->val;
    }

    template <typename K, typename V>
    void lru_cache<K, V>::put(const K& key, const V& val) {
        auto it = cache_map.find(key);
        if (it != cache_map.end()) {
            it->second->val = val;
            move_to_front(it->second);
            return;
        }
        if (is_full()) evict_lru();
        node_l<K, V>* new_node = new node_l<K, V>(key, val);
        insert_at_front(new_node);
        cache_map[key] = new_node;
        ++count;    
    }

    template <typename K, typename V>
    void lru_cache<K, V>::remove(const K& key) {
        auto it = cache_map.find(key);
        if (it == cache_map.end())
            throw std::out_of_range("remove: key not found");
        detach(it->second);
        delete it->second;
        cache_map.erase(it);
        --count;
    }

    template <typename K, typename V>
    bool lru_cache<K, V>::contains(const K& key) const {
        return cache_map.find(key) != cache_map.end();  
    }

    template <typename K, typename V>
    K lru_cache<K, V>::peek_lru() const {
        if (tail == nullptr)
            throw std::underflow_error("peek_lru: cache is empty");
        return tail->key;   
    }

    template <typename K, typename V>
    K lru_cache<K, V>::peek_mru() const {
        if (head == nullptr)
            throw std::underflow_error("peek_mru: cache is empty");
        return head->key;
    }

    template <typename K, typename V>
    void lru_cache<K, V>::print() const {
        std::cout << "lru_cache [" << count << "/" << cap << "] : ";
        node_l<K, V>* curr = head;
        
        while (curr != nullptr) {
            std::cout << "|" << curr->key << ":" << curr->val << "|";
            if (curr->next) std::cout << " <-> ";
            curr = curr->next;
        }
        std::cout << "  (MRU -> LRU)\n";
    }
}