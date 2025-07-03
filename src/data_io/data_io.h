#pragma once

#include <string>

#include "models/linalg/csr.h"

using std::string;

namespace data_io {
    /**
     * Loads segment data from a JSON file into a CSR matrix representing segment connectivity.
     *
     * @param filename The path to the JSON file containing segment data.
     * @return A CsrMatrix representing the adjacency matrix of segment connections.
     *
     * @throws std::runtime_error if the file cannot be opened or parsed, or if matrix construction fails.
     */
    linalg::CsrMatrix loadSegmentsMatrix(const string &filename);
}
