#include <vector>

#include "catch/catch.hpp"

#include "models/listWithSize.h"
#include "models/linalg/csr.h"

using linalg::CsrMatrix;
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