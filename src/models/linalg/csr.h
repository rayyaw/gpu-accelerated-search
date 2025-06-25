#pragma once

#include <vector>

#include "models/listWithSize.h"

using std::vector;
using utils::ListWithSize;

namespace linalg {
    class CsrMatrix {
        public:
        // Input is of the form matrix[row[col]]
        // Values should be in seconds
        CsrMatrix(const vector<vector<uint16_t>> &matrix);

        // Element access: matrix(row, col)
        uint16_t operator()(size_t row, size_t col) const;

        private:
        size_t _rows = 0;
        size_t _cols = 0;

        ListWithSize<uint16_t> _values = ListWithSize<uint16_t>(0);
        ListWithSize<size_t> _col_indices = ListWithSize<size_t>(0);
        ListWithSize<size_t> _row_ptr = ListWithSize<size_t>(0);
    };
}