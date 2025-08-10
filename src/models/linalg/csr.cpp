#include <vector>

#include "csr.h"

using linalg::CsrMatrix;
using linalg::MutableCsrMatrix;
using std::pair;
using std::vector;
using utils::ListWithSize;
using utils::MemoryResult;

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

CsrMatrix::CsrMatrix(const MutableCsrMatrix &mutableMatrix) {
    _rows = mutableMatrix._rows;
    _cols = mutableMatrix._cols;
    _values = ListWithSize<uint16_t>(mutableMatrix._values);
    _col_indices = ListWithSize<size_t>(mutableMatrix._col_indices);
    _row_ptr = ListWithSize<size_t>(mutableMatrix._row_ptr);
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

pair<MemoryResult<uint16_t>, MemoryResult<size_t>> CsrMatrix::valuesInRow(size_t row) const {
    size_t row_start = _row_ptr[row];
    size_t next_row_start = _row_ptr[row + 1];

    uint16_t *row_start_ref_val = _values.data() + row_start;
    size_t *row_start_ref_col = _col_indices.data() + row_start;

    MemoryResult values = MemoryResult<uint16_t>(next_row_start - row_start, row_start_ref_val);
    MemoryResult columns = MemoryResult<size_t>(next_row_start - row_start, row_start_ref_col);

    return {values, columns};
}

size_t CsrMatrix::getMemorySize() const {
    size_t values_size = _values.size() * sizeof(uint16_t);
    size_t col_indices_size = _col_indices.size() * sizeof(size_t);
    size_t row_ptr_size = _row_ptr.size() * sizeof(size_t);
    return values_size + col_indices_size + row_ptr_size;
}

MutableCsrMatrix::MutableCsrMatrix(size_t rows, size_t cols) {
    _rows = rows;
    _cols = cols;

    // This needs to be all 0 of size rows + 1
    _row_ptr = vector<size_t>(rows + 1, 0);
}

void MutableCsrMatrix::addEntry(size_t row, size_t col, uint16_t entry) {
    // Find the insertion position: at the end of the given row segment
    size_t insert_pos = _row_ptr[row + 1];

    // Insert the entry and column index at insert_pos
    _values.insert(_values.begin() + insert_pos, entry);
    _col_indices.insert(_col_indices.begin() + insert_pos, col);

    // Update _row_ptr for all subsequent rows (shift by +1)
    for (size_t r = row + 1; r < _row_ptr.size(); ++r) {
        _row_ptr[r]++;
    }
}

uint16_t MutableCsrMatrix::operator()(const size_t row, const size_t col) const {
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

size_t MutableCsrMatrix::getMemorySize() const {
    size_t values_size = _values.size() * sizeof(uint16_t);
    size_t col_indices_size = _col_indices.size() * sizeof(size_t);
    size_t row_ptr_size = _row_ptr.size() * sizeof(size_t);
    return values_size + col_indices_size + row_ptr_size;
}
