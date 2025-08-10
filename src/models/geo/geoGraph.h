#pragma once

#include <optional>
#include <string>
#include <vector>

#include "external/nlohmann/json.hpp"
#include "models/geo/geoData.h"
#include "models/linalg/csr.h"
#include "models/util/dim2Tree.h"

using geo::GraphNode;
using linalg::CsrMatrix;
using nlohmann::json;
using std::optional;
using std::string;
using std::vector;
using utils::Dim2Tree;

namespace geo {
    class Graph {
        public:
        Graph(const string &filename);
        Graph(vector<GraphNode> nodes);

        optional<vector<GraphNode>> generateRoute(
            pair<float, float> origin,
            pair<float, float> dest
        );

        // These fields should be treated as private outside of unit tests
        Dim2Tree _vertices;
        // Each edge length is a timestamp
        CsrMatrix _edges;

        private:
        void loadGraphFromNodes(vector<GraphNode> nodes);
        json loadFileAsJson(const string &filename);
        vector<GraphNode> parseGraphNodes(const json &j);
    };
}