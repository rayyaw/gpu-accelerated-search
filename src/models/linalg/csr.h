#pragma once

#include <vector>

using std::vector;

namespace linalg {
    template<typename T>
    class MutableCsrMatrix<T> {
        public:
        MutableCsrMatrix<T>(size_t rows, size_t cols);
        static MutableCsrMatrix<T> fromDenseMatrix(const vector<vector<T>> &matrix);
        CsrMatrix<T> toImmutable() const;

        // Element access: matrix(row, col)
        T &operator()(size_t row, size_t col);

        private:
        size_t _rows;
        size_t _cols;

        vector<T> _values;
        vector<T> _col_indices;
        vector<T> _row_ptr;
    }

    template<typename T>
    class CsrMatrix<T> {
        public:
        static CsrMatrix<T> fromDenseMatrix(const vector<vector<T>> &matrix);

        // Element access: matrix(row, col)
        T &operator()(size_t row, size_t col) const;

        private:
        size_t _rows;
        size_t _cols;

        ListWithSize<T> _values;
        ListWithSize<T> _col_indices;
        ListWithSize<T> _row_ptr;
    }
}