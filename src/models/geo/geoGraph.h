#pragma once

#include <vector>

#include "models/geo/geoData.h"
#include "models/linalg/csr.h"
#include "models/util/dim2Tree.h"

using geo::GraphNode;
using linalg::CsrMatrix;
using utils::Dim2Tree;

namespace geo {
    class Graph {
        public:
        Graph(vector<GraphNode> nodes);

        private:
        Dim2Tree _vertices;

        // Each edge length is a timestamp
        CsrMatrix _edges;
    }
}