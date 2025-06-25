#pragma once

#include <format>
#include <stdexcept>
#include <vector>

using std::format;
using std::out_of_range;
using std::vector;

namespace utils {
    template <typename T>

    // Simple struct for returning a list's size along with the list itself
    // Managing the number of items is left to the user, and all functions implicitly assume it is correct
    class ListWithSize {
        public:
        ListWithSize(size_t num_items);
        ListWithSize(size_t num_items, T* list_items);
        ListWithSize(vector<T> items);
        ListWithSize(const ListWithSize &other);
        ListWithSize &operator=(const ListWithSize &other);
        ~ListWithSize();

        T &operator[](size_t index) const;
        size_t size() const;

        private:
        size_t _num_items;

        // Future optimization: Pass a constructor option that makes this live in pinned memory
        T* _items;
    };

    template<typename T>
    ListWithSize<T>::ListWithSize(size_t num_items) {
        _num_items = num_items;
        _items = new T[num_items];
    }

    template<typename T>
    ListWithSize<T>::ListWithSize(size_t num_items, T* list_items) {
        _num_items = num_items;
        _items = new T[_num_items];

        std::copy(list_items, list_items + _num_items, _items);
    }

    template<typename T>
    ListWithSize<T>::ListWithSize(vector<T> items) {
        _num_items = items.size();
        _items = new T[_num_items];

        T* data = items.data();

        std::copy(data, data + _num_items, _items);
    }

    template<typename T>
    ListWithSize<T>::ListWithSize(const ListWithSize<T> &other) {
        _num_items = other._num_items;
        _items = new T[_num_items];

        std::copy(other._items, other._items + _num_items, _items);
    }

    template<typename T>
    ListWithSize<T> &ListWithSize<T>::operator=(const ListWithSize<T> &other) {
        if (this != &other) {
            _num_items = other._num_items;
            _items = new T[_num_items];

            std::copy(other._items, other._items + _num_items, _items);
        }

        return *this;
    }

    template<typename T>
    ListWithSize<T>::~ListWithSize() {
        delete[] _items;
    }

    template<typename T>
    T &ListWithSize<T>::operator[](size_t index) const {
        if (index >= _num_items) {
            throw out_of_range(format("Attempt to access element {} in ListWithSize of size {}", index, _num_items));
        }

        return _items[index];
    }

    template<typename T>
    size_t ListWithSize<T>::size() const {
        return _num_items;
    }
}