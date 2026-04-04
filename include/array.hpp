#pragma once
#include <iostream>
#include <stdexcept>
#include <algorithm>

namespace cds {
    template <typename T>
    class array {
    public:
        explicit array(int size_);
        array(const array& other);
        array& operator=(const array& other);
        ~array();

        void print() const;

        bool linear_search(const T& val) const;         
        bool bidirectional_search(const T& val) const;  
        bool binary_search(const T& val) const;         

        bool is_sorted() const;
        int  size() const noexcept { return size_; }

        T& operator[](int index);
        const T& operator[](int index) const;

        void bubble_sort();              
        void bubble_sort_optimized();    
        void selection_sort();           
        void insertion_sort();           
        void shell_sort();               
        void merge_sort();               
        void quick_sort();               

    private:
        int size_;    
        T*  data;     

        void merge_sort_rec(int left, int right);
        void merge(int left, int mid, int right);
        void quick_sort_rec(int left, int right);
        int  partition(int left, int right);
    };

    template <typename T>
    array<T>::array(int size_) : size_(size_), data(new T[size_]) {
        if(size_ <= 0) {
            delete[] data;
            throw std::invalid_argument("size_ must be positive");
        }
    }

    template <typename T>
    array<T>::array(const array& other) : size_(other.size_), data(new T[other.size_]) {
        for (int i = 0; i < size_; ++i) data[i] = other.data[i];
    }

    template <typename T>
    array<T>& array<T>::operator=(const array& other) {
        if (this != &other) {
            delete[] data;
            size_ = other.size_;
            data = new T[size_];
            for (int i = 0; i < size_; ++i) data[i] = other.data[i];
        }
        return *this;
    }

    template <typename T>
    array<T>::~array() {
        delete[] data;
    }

    template <typename T>
    void array<T>::print() const {
        for (int i = 0; i < size_; ++i) std::cout << data[i] << " ";
        std::cout << "\n";
    }

    template <typename T>
    T& array<T>::operator[](int index) {
        if (index < 0 || index >= size_)
            throw std::out_of_range("array[]: index out of range");
        return data[index];
    }

    template <typename T>
    const T& array<T>::operator[](int index) const {
        if (index < 0 || index >= size_)
            throw std::out_of_range("array[]: index out of range");
        return data[index];
    }

    template <typename T>
    bool array<T>::linear_search(const T& val) const {
        for (int i = 0; i < size_; ++i)
            if (data[i] == val) return true;
        return false;
    }

    template <typename T>
    bool array<T>::bidirectional_search(const T& val) const {
        int left = 0, right = size_ - 1;
        while (left <= right) {
            if (data[left] == val || data[right] == val) return true;
            ++left; --right;
        }
        return false;
    }

    template <typename T>
    bool array<T>::binary_search(const T& val) const {
        int left = 0, right = size_ - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (data[mid] == val) return true;
            if (data[mid] < val) left = mid + 1;
            else right = mid - 1;
        }
        return false;
    }

    template <typename T>
    bool array<T>::is_sorted() const {
        for (int i = 0; i < size_ - 1; ++i)
            if (data[i] > data[i + 1]) return false; // FIXED BUG
        return true;
    }

    template <typename T>
    void array<T>::bubble_sort() {
        for (int i = 0; i < size_; ++i)
            for (int j = 0; j < size_ - 1 - i; ++j)
                if (data[j] > data[j + 1])
                    std::swap(data[j], data[j + 1]);
    }

    template <typename T>
    void array<T>::bubble_sort_optimized() {
        for (int i = 0; i < size_; ++i) {
            bool swapped = false;
            for (int j = 0; j < size_ - 1 - i; ++j)
                if (data[j] > data[j + 1]) {
                    std::swap(data[j], data[j + 1]);
                    swapped = true;
                }
            if (!swapped) break;
        }
    }

    template <typename T>
    void array<T>::selection_sort() {
        for (int i = 0; i < size_ - 1; ++i) {
            int min_idx = i;
            for (int j = i + 1; j < size_; ++j)
                if (data[j] < data[min_idx]) min_idx = j;
            std::swap(data[i], data[min_idx]);
        }
    }

    template <typename T>
    void array<T>::insertion_sort() {
        for (int i = 1; i < size_; ++i) {
            T key = data[i];
            int j = i - 1;
            while (j >= 0 && data[j] > key) {
                data[j + 1] = data[j];
                --j;
            }
            data[j + 1] = key;
        }
    }

    template <typename T>
    void array<T>::shell_sort() {
        for (int gap = size_ / 2; gap > 0; gap /= 2)
            for (int i = gap; i < size_; ++i) {
                T key = data[i];
                int j = i;
                while (j >= gap && data[j - gap] > key) {
                    data[j] = data[j - gap];
                    j -= gap;
                }
                data[j] = key;
            }
    }

    template <typename T>
    void array<T>::merge(int left, int mid, int right) {
        int n1 = mid - left + 1, n2 = right - mid;

        T* L = new T[n1];
        T* R = new T[n2];

        for (int i = 0; i < n1; ++i) L[i] = data[left + i];
        for (int j = 0; j < n2; ++j) R[j] = data[mid + 1 + j];

        int i = 0, j = 0, k = left;

        while (i < n1 && j < n2)
            data[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];

        while (i < n1) data[k++] = L[i++];
        while (j < n2) data[k++] = R[j++];

        delete[] L;
        delete[] R;
    }

    template <typename T>
    void array<T>::merge_sort_rec(int left, int right) {
        if (left >= right) return;

        int mid = left + (right - left) / 2;

        merge_sort_rec(left, mid);
        merge_sort_rec(mid + 1, right);
        merge(left, mid, right);
    }

    template <typename T>
    void array<T>::merge_sort() {
        merge_sort_rec(0, size_ - 1);
    }

    template <typename T>
    int array<T>::partition(int left, int right) {
        T pivot = data[right];
        int i = left - 1;

        for (int j = left; j < right; ++j) {
            if (data[j] <= pivot) {
                ++i;
                std::swap(data[i], data[j]);
            }
        }

        std::swap(data[i + 1], data[right]);
        return i + 1;
    }

    template <typename T>
    void array<T>::quick_sort_rec(int left, int right) {
        if (left >= right) return;

        int pi = partition(left, right);

        quick_sort_rec(left, pi - 1);
        quick_sort_rec(pi + 1, right);
    }

    template <typename T>
    void array<T>::quick_sort() {
        quick_sort_rec(0, size_ - 1);
    }
}