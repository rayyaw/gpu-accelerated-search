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
    vector<vector<uint16_t>> denseMatrix = {
        {0, 0, 1, 0},
        {1, 1, 0, 1},
        {0, 0, 0, 0},
        {0, 1, 0, 0}
    };

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