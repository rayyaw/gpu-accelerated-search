#pragma once

#include <unordered_map>

#include "models/geo/geoData.h"
#include "models/linalg/csr.h"
#include "models/util/dim2Tree.h"

using geo::GraphEdge;
using std::unordered_map;
using linalg::CsrMatrix;
using utils::Dim2Tree;

namespace geo {
    class Graph {
        public:
        Graph() = default;
        void addEdge(GraphEdge g);

        private:
        Dim2Tree vertices;

        // Each edge length is a timestamp
        CsrMatrix edges;

        // Need bi-directional lookup for efficiency
        // Maps array indices (in the Dim2Tree) to OpenStreetMap Segment IDs
        unordered_map<size_t, string> segment_id_mappings = unordered_map<size_t, string>();
        unordered_map<string, size_t> reverse_segment_id_mappings = unordered_map<string, size_t>();
    }
}