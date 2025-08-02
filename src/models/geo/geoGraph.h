#pragma once

#include <string>
#include <vector>

#include "external/nlohmann/json.hpp"
#include "models/geo/geoData.h"
#include "models/linalg/csr.h"
#include "models/util/dim2Tree.h"

using geo::GraphNode;
using linalg::CsrMatrix;
using nlohmann::json;
using std::string;
using utils::Dim2Tree;

namespace geo {
    class Graph {
        public:
        Graph(const string &filename);
        Graph(vector<GraphNode> nodes);

        private:
        void loadGraphFromNodes(vector<GraphNode> nodes);
        json loadFileAsJson(const string &filename);
        vector<GraphNode> parseGraphNodes(const json &j);

        Dim2Tree _vertices;

        // Each edge length is a timestamp
        CsrMatrix _edges;
    };
}