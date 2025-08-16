#pragma once

#include <format>
#include <iostream>
#include <stdexcept>
#include <vector>

using std::format;
using std::istream;
using std::ostream;
using std::out_of_range;
using std::vector;

namespace utils {
    template <typename T>
    // Same as ListWithSize, but memory is managed externally to the object.
    // Do not attempt cleanup on a MemoryResult unless the generator explicitly declares that operation as safe.
    struct MemoryResult {
        size_t num_items;
        T* items;
    };

    template <typename T>
    // Simple struct for returning a list's size along with the list itself
    // Managing the number of items is left to the user, and all functions implicitly assume it is correct
    // This class manages the items for you, so no need to worry about pointer management.
    // If created from a T*, that T* remains your responsibility to clean up.
    class ListWithSize {
        public:
        ListWithSize(size_t num_items);
        ListWithSize(size_t num_items, T* list_items);
        ListWithSize(vector<T> items);
        ListWithSize(const ListWithSize &other);
        ListWithSize &operator=(const ListWithSize &other);
        ~ListWithSize();

        T &operator[](size_t index) const;
        
        template<typename I>
        friend istream &operator>>(istream &input, ListWithSize<I> &list);

        template<typename O>
        friend ostream &operator<<(ostream &output, const ListWithSize<O> &list);

        size_t size() const;
        T* data() const;

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

    template<typename T>
    T* ListWithSize<T>::data() const {
        return _items;
    }

    template<typename I>
    istream &operator>>(istream& input, ListWithSize<I>& list) {
        delete[] list._items;

        input.read(reinterpret_cast<char*>(&list._num_items), sizeof(list._num_items));
        std::cout << "reading in a list of " << list._num_items << " items" << std::endl;
        I* items = new I[list._num_items];
        input.read(reinterpret_cast<char*>(items), list._num_items * sizeof(I));
        
        list._items = items;

        return input;
    }

    template<typename O>
    ostream &operator<<(ostream& output, const ListWithSize<O>& list) {
        std::cout << "writing out a list of " << list._num_items << " items each of size " << sizeof(O) << std::endl;
        output.write(reinterpret_cast<const char*>(&list._num_items), sizeof(list._num_items));
        output.write(reinterpret_cast<const char*>(list._items), list._num_items * sizeof(O));

        return output;
    }
}

