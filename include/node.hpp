#pragma once

namespace cds {
    template <typename T>
    struct node_s {
        T data;
        node_s* next;
        explicit node_s(const T& val) : data(val), next(nullptr) {}
    };

    template <typename T>
    struct node_d {
        T data;
        node_d* next;
        node_d* prev;
        explicit node_d(const T& val) : data(val), next(nullptr), prev(nullptr) {}
    };

    template <typename T>
    struct node_b {
        T data;
        node_b* left;
        node_b* right;
        explicit node_b(const T& val) : data(val), left(nullptr), right(nullptr) {}
    };

    template <typename K, typename V>
    struct node_h {
        K key;
        V val;
        node_h* next;
        explicit node_h(const K& k, const V& v) : key(k), val(v), next(nullptr) {}
    };
}