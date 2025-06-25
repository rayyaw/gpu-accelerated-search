#include <vector>

#include "csr.h"

using linalg::CsrMatrix;
using std::vector;
using utils::ListWithSize;

CsrMatrix::CsrMatrix(const vector<vector<uint16_t>> &matrix) {
    _rows = matrix.size();
    _cols = matrix[0].size();

    vector<uint16_t> data = vector<uint16_t>();
    vector<size_t> cols = vector<size_t>();
    vector<size_t> rowptr = vector<size_t>();

    size_t entry_counter = 0;
    rowptr.push_back(entry_counter);

    for (vector<uint16_t> row : matrix) {
        for (size_t i = 0; i < row.size(); i++) {
            if (row[i] != 0) {
                data.push_back(row[i]);
                cols.push_back(i);
                entry_counter++;
            }
        }

        rowptr.push_back(entry_counter);
    }

    _values = ListWithSize<uint16_t>(data);
    _col_indices = ListWithSize<size_t>(cols);
    _row_ptr = ListWithSize<size_t>(rowptr);
}

uint16_t CsrMatrix::operator()(const size_t row, const size_t col) const {
    size_t row_start = _row_ptr[row];
    size_t next_row_start = _row_ptr[row + 1];

    for (size_t i = row_start; i < next_row_start; i++) {
        size_t current_col = _col_indices[i];

        if (current_col == col) {
            return _values[i];
        }
    }

    return (uint16_t) 0;
}