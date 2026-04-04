#pragma once
#include "node.hpp"
#include <iostream>
#include <queue>
#include <stdexcept>
#include <algorithm>
#include <string>

namespace cds {
    template <typename T>
    class bst {
    public:
        bst();
        bst(const bst& other);
        bst& operator=(const bst& other);
        ~bst();

        void insert(const T& val);
        void erase(const T& val);
        bool contains(const T& val) const;

        void inorder() const;
        void preorder() const;
        void postorder() const;
        void level_order() const;

        int size() const;
        int leaf_count() const;
        int parent_count() const;
        int height() const;
        bool is_balanced() const;
        bool empty() const noexcept;

    private:
        node_b<T>* root;

        void insert_rec(node_b<T>*& node, const T& val);
        node_b<T>* erase_rec(node_b<T>* node, const T& val);
        bool search_rec(node_b<T>* node, const T& val) const;

        void inorder_rec(node_b<T>* n) const;
        void preorder_rec(node_b<T>* n) const;
        void postorder_rec(node_b<T>* n) const;

        int count_rec(node_b<T>* n) const;
        int leaf_rec(node_b<T>* n) const;
        int height_rec(node_b<T>* n) const;
        int balance_rec(node_b<T>* n) const;
        node_b<T>* copy_rec(node_b<T>* n) const;
        void destroy(node_b<T>* n);
        
        node_b<T>* extract_min(node_b<T>*& node); 
    };

    template <typename T>
    bst<T>::bst() : root(nullptr) {}

    template <typename T>
    bst<T>::bst(const bst& other) : root(copy_rec(other.root)) {}

    template <typename T>
    bst<T>& bst<T>::operator=(const bst& other) {
        if (this != &other) {
            destroy(root);
            root = copy_rec(other.root);
        }
        return *this;
    }

    template <typename T>
    bst<T>::~bst() {
        destroy(root);
        root = nullptr;
    }

    template <typename T>
    void bst<T>::insert(const T& val) {
        insert_rec(root, val);
    }

    template <typename T>
    void bst<T>::erase(const T& val) {
        root = erase_rec(root, val);
    }

    template <typename T>
    bool bst<T>::contains(const T& val) const {
        return search_rec(root, val);
    }

    template <typename T>
    void bst<T>::inorder() const {
        inorder_rec(root);
        std::cout << std::endl;
    }

    template <typename T>
    void bst<T>::preorder() const {
        preorder_rec(root);
        std::cout << std::endl;
    }

    template <typename T>
    void bst<T>::postorder() const {
        postorder_rec(root);
        std::cout << std::endl;
    }

    template <typename T>
    void bst<T>::level_order() const {
        if (root == nullptr) return;

        std::queue<node_b<T>*> q;
        q.push(root);
        while (!q.empty()) {
            node_b<T>* cur = q.front(); q.pop();
            std::cout << cur->data << " ";
            if (cur->left)  q.push(cur->left);
            if (cur->right) q.push(cur->right);
        }
        std::cout << std::endl;
    }

    template <typename T>
    int bst<T>::size() const {
        return count_rec(root);
    }

    template <typename T>
    int bst<T>::leaf_count() const {
        return leaf_rec(root);
    }

    template <typename T>
    int bst<T>::parent_count() const {
        int c = size() - leaf_count();
        return (c < 0) ? 0 : c;
    }

    template <typename T>
    int bst<T>::height() const {
        return height_rec(root);
    }

    template <typename T>
    bool bst<T>::is_balanced() const {
        return balance_rec(root) != -1;
    }

    template <typename T>
    bool bst<T>::empty() const noexcept {
        return root == nullptr;
    }

    template <typename T>
    void bst<T>::insert_rec(node_b<T>*& node, const T& val) {
        if (node == nullptr) {
            node = new node_b<T>(val);
            return;
        }
        if (val == node->data) {
            throw std::invalid_argument("insert: duplicate value encountered");
        }
        if (val < node->data)
            insert_rec(node->left, val);
        else
            insert_rec(node->right, val);
    }

    template <typename T>
    node_b<T>* bst<T>::extract_min(node_b<T>*& node) {
        if (node->left == nullptr) {
            node_b<T>* min_node = node;
            node = node->right;
            return min_node;
        }
        return extract_min(node->left);
    }

    template <typename T>
    node_b<T>* bst<T>::erase_rec(node_b<T>* node, const T& val) {
        if (node == nullptr) return nullptr;

        if (val < node->data) {
            node->left = erase_rec(node->left, val);
        } else if (val > node->data) {
            node->right = erase_rec(node->right, val);
        } else {
            if (node->left == nullptr)  { node_b<T>* t = node->right; delete node; return t; }
            if (node->right == nullptr) { node_b<T>* t = node->left;  delete node; return t; }

            node_b<T>* succ = extract_min(node->right);
            succ->left = node->left;
            succ->right = node->right;
            delete node;
            return succ;
        }
        return node;
    }

    template <typename T>
    bool bst<T>::search_rec(node_b<T>* node, const T& val) const {
        if (node == nullptr) return false;
        if (val == node->data) return true;
        return val < node->data ? search_rec(node->left, val) : search_rec(node->right, val);
    }

    template <typename T>
    void bst<T>::inorder_rec(node_b<T>* n) const {
        if (!n) return;
        inorder_rec(n->left);
        std::cout << n->data << " ";
        inorder_rec(n->right);
    }

    template <typename T>
    void bst<T>::preorder_rec(node_b<T>* n) const {
        if (!n) return;
        std::cout << n->data << " ";
        preorder_rec(n->left);
        preorder_rec(n->right);
    }

    template <typename T>
    void bst<T>::postorder_rec(node_b<T>* n) const {
        if (!n) return;
        postorder_rec(n->left);
        postorder_rec(n->right);
        std::cout << n->data << " ";
    }

    template <typename T>
    int bst<T>::count_rec(node_b<T>* n) const {
        return n ? 1 + count_rec(n->left) + count_rec(n->right) : 0;
    }

    template <typename T>
    int bst<T>::leaf_rec(node_b<T>* n) const {
        if (n == nullptr) return 0;
        if (!n->left && !n->right) return 1;
        return leaf_rec(n->left) + leaf_rec(n->right);
    }

    template <typename T>
    int bst<T>::height_rec(node_b<T>* n) const {
        return n ? 1 + std::max(height_rec(n->left), height_rec(n->right)) : 0;
    }

    template <typename T>
    int bst<T>::balance_rec(node_b<T>* n) const {
        if (n == nullptr) return 0;

        int lh = balance_rec(n->left);
        if (lh == -1) return -1;

        int rh = balance_rec(n->right);
        if (rh == -1) return -1;

        if (std::abs(lh - rh) > 1) return -1;
        return 1 + std::max(lh, rh);
    }

    template <typename T>
    node_b<T>* bst<T>::copy_rec(node_b<T>* n) const {
        if (n == nullptr) return nullptr;
        node_b<T>* newn = new node_b<T>(n->data);
        newn->left = copy_rec(n->left);
        newn->right = copy_rec(n->right);
        return newn;
    }

    template <typename T>
    void bst<T>::destroy(node_b<T>* n) {
        if (n == nullptr) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }
};