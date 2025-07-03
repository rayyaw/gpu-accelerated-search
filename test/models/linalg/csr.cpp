#include <vector>

#include "catch/catch.hpp"

#include "models/linalg/csr.h"
#include "models/util/listWithSize.h"

using linalg::CsrMatrix;
using linalg::MutableCsrMatrix;
using std::vector;
using utils::ListWithSize;

TEST_CASE("Standard CsrMatrix initialization and access", "[CsrMatrix]") {
    vector<vector<uint16_t>> denseMatrix = {
        {0, 0, 1, 0},
        {1, 1, 0, 1},
        {0, 0, 0, 0},
        {0, 1, 0, 0}
    };

    CsrMatrix sparseMatrix = CsrMatrix(denseMatrix);

    REQUIRE(sparseMatrix(0, 0) == 0);
    REQUIRE(sparseMatrix(0, 1) == 0);
    REQUIRE(sparseMatrix(0, 2) == 1);
    REQUIRE(sparseMatrix(0, 3) == 0);

    REQUIRE(sparseMatrix(1, 0) == 1);
    REQUIRE(sparseMatrix(1, 1) == 1);
    REQUIRE(sparseMatrix(1, 2) == 0);
    REQUIRE(sparseMatrix(1, 3) == 1);

    REQUIRE(sparseMatrix(2, 0) == 0);
    REQUIRE(sparseMatrix(2, 1) == 0);
    REQUIRE(sparseMatrix(2, 2) == 0);
    REQUIRE(sparseMatrix(2, 3) == 0);

    REQUIRE(sparseMatrix(3, 0) == 0);
    REQUIRE(sparseMatrix(3, 1) == 1);
    REQUIRE(sparseMatrix(3, 2) == 0);
    REQUIRE(sparseMatrix(3, 2) == 0);
}

TEST_CASE("Standard MutableCsrMatrix initialization and access", "[MutableCsrMatrix]") {
    MutableCsrMatrix sparseMatrix = MutableCsrMatrix(4, 4);
    sparseMatrix.addEntry(0, 2, 1);
    sparseMatrix.addEntry(1, 0, 1);
    sparseMatrix.addEntry(1, 1, 1);
    sparseMatrix.addEntry(1, 3, 1);
    sparseMatrix.addEntry(3, 1, 1);

    REQUIRE(sparseMatrix(0, 0) == 0);
    REQUIRE(sparseMatrix(0, 1) == 0);
    REQUIRE(sparseMatrix(0, 2) == 1);
    REQUIRE(sparseMatrix(0, 3) == 0);

    REQUIRE(sparseMatrix(1, 0) == 1);
    REQUIRE(sparseMatrix(1, 1) == 1);
    REQUIRE(sparseMatrix(1, 2) == 0);
    REQUIRE(sparseMatrix(1, 3) == 1);

    REQUIRE(sparseMatrix(2, 0) == 0);
    REQUIRE(sparseMatrix(2, 1) == 0);
    REQUIRE(sparseMatrix(2, 2) == 0);
    REQUIRE(sparseMatrix(2, 3) == 0);

    REQUIRE(sparseMatrix(3, 0) == 0);
    REQUIRE(sparseMatrix(3, 1) == 1);
    REQUIRE(sparseMatrix(3, 2) == 0);
    REQUIRE(sparseMatrix(3, 2) == 0);
}

TEST_CASE("Conversion from MutableCsrMatrix to CsrMatrix", "[CsrMatrix][MutableCsrMatrix]") {
    // Create a MutableCsrMatrix with a known structure
    MutableCsrMatrix mutableMatrix = MutableCsrMatrix(4, 4);
    mutableMatrix.addEntry(0, 2, 1);
    mutableMatrix.addEntry(1, 0, 1);
    mutableMatrix.addEntry(1, 1, 1);
    mutableMatrix.addEntry(1, 3, 1);
    mutableMatrix.addEntry(3, 1, 1);

    // Convert to CsrMatrix
    CsrMatrix immutableMatrix = CsrMatrix(mutableMatrix);

    // Verify that the data is preserved after conversion
    REQUIRE(immutableMatrix(0, 0) == 0);
    REQUIRE(immutableMatrix(0, 1) == 0);
    REQUIRE(immutableMatrix(0, 2) == 1);
    REQUIRE(immutableMatrix(0, 3) == 0);

    REQUIRE(immutableMatrix(1, 0) == 1);
    REQUIRE(immutableMatrix(1, 1) == 1);
    REQUIRE(immutableMatrix(1, 2) == 0);
    REQUIRE(immutableMatrix(1, 3) == 1);

    REQUIRE(immutableMatrix(2, 0) == 0);
    REQUIRE(immutableMatrix(2, 1) == 0);
    REQUIRE(immutableMatrix(2, 2) == 0);
    REQUIRE(immutableMatrix(2, 3) == 0);

    REQUIRE(immutableMatrix(3, 0) == 0);
    REQUIRE(immutableMatrix(3, 1) == 1);
    REQUIRE(immutableMatrix(3, 2) == 0);
    REQUIRE(immutableMatrix(3, 3) == 0);
}
