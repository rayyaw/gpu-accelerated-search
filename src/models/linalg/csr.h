#pragma once

#include <vector>

#include "models/util/listWithSize.h"

using std::vector;
using utils::ListWithSize;

namespace linalg {
    class MutableCsrMatrix {
        public:
        // Input is of the form matrix[row[col]]
        // Values should be in seconds
        MutableCsrMatrix(size_t rows, size_t cols);
        void addEntry(size_t row, size_t col, uint16_t entry);
        size_t getMemorySize() const;

        // Element access: matrix(row, col)
        uint16_t operator()(size_t row, size_t col) const;

        private:
        friend class CsrMatrix; // Allow CsrMatrix to access private members for conversion
        size_t _rows = 0;
        size_t _cols = 0;

        // so we need an array of locations (double pairs), plus a way to snap to the nearest one
        // this has to be a presorted KD tree which needs to be loaded from a file
        vector<uint16_t> _values = vector<uint16_t>();
        vector<size_t> _col_indices = vector<size_t>();
        vector<size_t> _row_ptr = vector<size_t>();
    };

    /**
    * CSR matrices. No guarantee of sorted order within a single row
    */
    class CsrMatrix {
        public:
        // Input is of the form matrix[row[col]]
        // Values should be in seconds
        CsrMatrix(const vector<vector<uint16_t>> &matrix);
        
        // Construct from a MutableCsrMatrix by copying its data
        CsrMatrix(const MutableCsrMatrix &mutableMatrix);

        // Element access: matrix(row, col)
        uint16_t operator()(size_t row, size_t col) const;
        
        // Calculate and return the memory size of the CSR matrix in bytes
        size_t getMemorySize() const;

        private:
        size_t _rows = 0;
        size_t _cols = 0;

        // so we need an array of locations (double pairs), plus a way to snap to the nearest one
        // this has to be a presorted KD tree which needs to be loaded from a file
        ListWithSize<uint16_t> _values = ListWithSize<uint16_t>(0);
        ListWithSize<size_t> _col_indices = ListWithSize<size_t>(0);
        ListWithSize<size_t> _row_ptr = ListWithSize<size_t>(0);
    };
}
