#pragma once 
#include "node.hpp"
#include <iostream>
#include <stdexcept>
#include <string> 
#include <initializer_list>
#include <utility>

namespace cds {
    template <typename K, typename V>
    class hash_map {
        public:
            hash_map();
            hash_map(std::initializer_list<std::pair<K, V>> init);
            hash_map(const hash_map& other);
            hash_map(hash_map&& other) noexcept;
            hash_map& operator = (const hash_map& other);
            hash_map& operator = (hash_map&& other) noexcept;
            ~hash_map();

            void insert(const K& key, const V& val);
            void insert_or_assign(const K& key, const V& val);
            V& get(const K& key);
            const V& get(const K& key) const;
            V& at(const K& key);
            const V& at(const K& key) const;
            void erase(const K& key);
            bool contains(const K& key) const;
            int count(const K& key) const;

            V* find(const K& key);
            const V* find(const K& key) const;

            int size() const noexcept { return cnt; }
            bool empty() const noexcept { return cnt == 0; }
            int bucket_count() const noexcept { return capacity; }
            float load_factor() const noexcept { return static_cast<float>(cnt) / capacity; }
            float max_load_factor() const noexcept { return max_load; }
            void max_load_factor(float f);
            void reserve(int n);
            void clear();

            V& operator[](const K& key);
            bool operator == (const hash_map& other) const;
            bool operator != (const hash_map& other) const;

            void print() const;
        
        private:
            node_h<K, V>** buckets;
            int capacity;
            int cnt;
            float max_load;

            static constexpr int INITIAL_CAPACITY = 16;
            static constexpr float DEFAULT_MAX_LOAD = 0.75f;

            int bucket_index(const K& key) const;
            void rehash();
            void copy_from(const hash_map& other);
            void destroy();
    };

    template <typename K>
    int hash_key(const K& key, int capacity) {
        return static_cast<int>(static_cast<unsigned long long>(key) % capacity);
    }

    template <>
    int hash_key(const std::string& key, int capacity) {
        unsigned long long hash = 5381;
        for (char c : key)
            hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
        return static_cast<int>(hash % capacity);
    }

    template <typename K, typename V>
    int hash_map<K, V>::bucket_index(const K& key) const {
        return hash_key(key, capacity);
    }

    template <typename K, typename V>
    void hash_map<K, V>::destroy() {
        for (int i = 0; i < capacity; ++i) {
            node_h<K, V>* curr = buckets[i];
            while (curr != nullptr) {
                node_h<K, V>* temp = curr;
                curr = curr->next;
                delete temp;
            }
            buckets[i] = nullptr;
        }
        cnt = 0;
    }

    template <typename K, typename V>
    void hash_map<K, V>::copy_from(const hash_map& other) {
        for (int i = 0; i < other.capacity; ++i) {
            node_h<K, V>* curr = other.buckets[i];
            while (curr != nullptr) {
                insert(curr->key, curr->val);
                curr = curr->next;
            }
        }
    }

    template <typename K, typename V>
    void hash_map<K, V>::rehash() {
        int old_capacity = capacity;
        node_h<K, V>** old_buckets = buckets;

        capacity *= 2;
        buckets = new node_h<K, V>*[capacity]();
        cnt = 0;
        
        for (int i = 0; i < old_capacity; ++i) {
            node_h<K, V>* curr = old_buckets[i];
            while (curr != nullptr) {
                insert(curr->key, curr->val);
                node_h<K, V>* temp = curr;
                curr = curr->next;
                delete temp;
            }
        }
        delete[] old_buckets;
    }

    template <typename K, typename V>
    hash_map<K, V>::hash_map() : capacity(INITIAL_CAPACITY), cnt(0), max_load(DEFAULT_MAX_LOAD) {
        buckets = new node_h<K, V>*[capacity]();
    }

    template <typename K, typename V>
    hash_map<K, V>::hash_map(std::initializer_list<std::pair<K, V>> init)
        : capacity(INITIAL_CAPACITY), cnt(0), max_load(DEFAULT_MAX_LOAD) {
        buckets = new node_h<K, V>*[capacity]();
        for (const auto& p : init)
            insert(p.first, p.second);
    }

    template <typename K, typename V>
    hash_map<K, V>::hash_map(const hash_map& other)
        : capacity(other.capacity), cnt(0), max_load(other.max_load) {
        buckets = new node_h<K, V>*[capacity]();
        copy_from(other);
    }

    template <typename K, typename V>
    hash_map<K, V>::hash_map(hash_map&& other) noexcept
        : buckets(other.buckets), capacity(other.capacity), cnt(other.cnt), max_load(other.max_load) {
        other.buckets = nullptr;
        other.capacity = 0;
        other.cnt = 0;
    }

    template <typename K, typename V>
    hash_map<K, V>& hash_map<K, V>::operator = (const hash_map& other) {
        if (this != &other) {
            destroy();
            delete[] buckets;
            capacity = other.capacity;
            max_load = other.max_load;
            buckets = new node_h<K, V>*[capacity]();
            copy_from(other);
        }
        return *this;
    }

    template <typename K, typename V>
    hash_map<K, V>& hash_map<K, V>::operator = (hash_map&& other) noexcept {
        if (this != &other) {
            destroy();
            delete[] buckets;
            buckets = other.buckets;
            capacity = other.capacity;
            cnt = other.cnt;
            max_load = other.max_load;
            other.buckets = nullptr;
            other.capacity = 0;
            other.cnt = 0;
        }
        return *this;
    }

    template <typename K, typename V>
    hash_map<K, V>::~hash_map() {
        destroy();
        delete[] buckets;
    }

    template <typename K, typename V>
    void hash_map<K, V>::max_load_factor(float f) {
        if (f <= 0.0f)
            throw std::invalid_argument("max_load_factor: must be > 0");
        max_load = f;
    }

    template <typename K, typename V>
    void hash_map<K, V>::reserve(int n) {
        int needed = static_cast<int>(n / max_load) + 1;
        if (needed <= capacity)
            return;

        int old_cap = capacity;
        node_h<K, V>** old_buckets = buckets;

        capacity = needed;
        buckets = new node_h<K, V>*[capacity]();
        cnt = 0;

        for (int i = 0; i < old_cap; ++i) {
            node_h<K, V>* curr = old_buckets[i];
            while (curr != nullptr) {
                insert(curr->key, curr->val);
                node_h<K, V>* temp = curr;
                curr = curr->next;
                delete temp;
            }
        }
        delete[] old_buckets;
    }

    template <typename K, typename V>
    void hash_map<K, V>::clear() {
        destroy();
    }

    template <typename K, typename V>
    void hash_map<K, V>::insert(const K& key, const V& val) {
        if (load_factor() >= max_load)
            rehash();

        int idx = bucket_index(key);
        node_h<K, V>* curr = buckets[idx];

        while (curr != nullptr) {
            if (curr->key == key) { curr->val = val; return; }
            curr = curr->next;
        }

        node_h<K, V>* e = new node_h<K, V>(key, val);
        e->next = buckets[idx];
        buckets[idx] = e;
        ++cnt;
    }

    template <typename K, typename V>
    void hash_map<K, V>::insert_or_assign(const K& key, const V& val) {
        insert(key, val);
    }

    template <typename K, typename V>
    V& hash_map<K, V>::get(const K& key) {
        int idx = bucket_index(key);
        for (node_h<K, V>* curr = buckets[idx]; curr != nullptr; curr = curr->next)
            if (curr->key == key) return curr->val;
        throw std::out_of_range("get: key not found");
    }

    template <typename K, typename V>
    const V& hash_map<K, V>::get(const K& key) const {
        int idx = bucket_index(key);
        for (node_h<K, V>* curr = buckets[idx]; curr != nullptr; curr = curr->next)
            if (curr->key == key) return curr->val;
        throw std::out_of_range("get: key not found");
    }

    template <typename K, typename V>
    V& hash_map<K, V>::at(const K& key) {
        int idx = bucket_index(key);
        for (node_h<K, V>* curr = buckets[idx]; curr != nullptr; curr = curr->next)
            if (curr->key == key) return curr->val;
        throw std::out_of_range("at: key not found");
    }

    template <typename K, typename V>
    const V& hash_map<K, V>::at(const K& key) const {
        int idx = bucket_index(key);
        for (node_h<K, V>* curr = buckets[idx]; curr != nullptr; curr = curr->next)
            if (curr->key == key) return curr->val;
        throw std::out_of_range("at: key not found");
    }

    template <typename K, typename V>
    void hash_map<K, V>::erase(const K& key) {
        int idx = bucket_index(key);
        node_h<K, V>* curr = buckets[idx];
        node_h<K, V>* prev = nullptr;

        while (curr != nullptr) {
            if (curr->key == key) {
                if (prev == nullptr) buckets[idx] = curr->next;
                else prev->next = curr->next;
                delete curr;
                --cnt;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
        throw std::out_of_range("erase: key not found");
    }

    template <typename K, typename V>
    bool hash_map<K, V>::contains(const K& key) const {
        int idx = bucket_index(key);
        for (node_h<K, V>* curr = buckets[idx]; curr != nullptr; curr = curr->next)
            if (curr->key == key) return true;
        return false;
    }

    template <typename K, typename V>
    int hash_map<K, V>::count(const K& key) const {
        return contains(key) ? 1 : 0;
    }

    template <typename K, typename V>
    V* hash_map<K, V>::find(const K& key) {
        int idx = bucket_index(key);
        for (node_h<K, V>* curr = buckets[idx]; curr != nullptr; curr = curr->next)
            if (curr->key == key) return &curr->val;
        return nullptr;
    }

    template <typename K, typename V>
    const V* hash_map<K, V>::find(const K& key) const {
        int idx = bucket_index(key);
        for (node_h<K, V>* curr = buckets[idx]; curr != nullptr; curr = curr->next)
            if (curr->key == key) return &curr->val;
        return nullptr;
    }

    template <typename K, typename V>
    V& hash_map<K, V>::operator[](const K& key) {
        if (!contains(key))
            insert(key, V{});
        return get(key);
    }

    template <typename K, typename V>
    bool hash_map<K, V>::operator == (const hash_map& other) const {
        if (cnt != other.cnt) return false;
        for (int i = 0; i < capacity; ++i)
            for (node_h<K, V>* curr = buckets[i]; curr != nullptr; curr = curr->next) {
                const V* v = other.find(curr->key);
                if (v == nullptr || *v != curr->val) return false;
            }
        return true;
    }

    template <typename K, typename V>
    bool hash_map<K, V>::operator != (const hash_map& other) const {
        return !(*this == other);
    }

    template <typename K, typename V>
    void hash_map<K, V>::print() const {
        for (int i = 0; i < capacity; ++i) {
            if (buckets[i] == nullptr) continue;
            std::cout << "  [" << i << "] -> ";
            for (node_h<K, V>* curr = buckets[i]; curr != nullptr; curr = curr->next)
                std::cout << "{" << curr->key << ":" << curr->val << "} ";
            std::cout << "\n";
        }
    }
} 