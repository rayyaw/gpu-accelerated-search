#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "catch/catch.hpp"
#include "models/geo/geoGraph.h"

using geo::Graph;
using geo::GraphNode;
using std::cout;
using std::endl;
using std::unordered_map;
using std::unordered_set;
using std::vector;

TEST_CASE("Graph neighbors are correctly preserved after Dim2Tree reordering", "[GeoGraph]") {
    // --- Step 1: Create known nodes
    vector<GraphNode> nodes(3);

    // Node 0 → neighbors: 1, 2
    nodes[0].nodeId = "0";
    nodes[0].location = {1.0f, 1.0f};
    nodes[0].outboundAccessibleNodes = {"1", "2"};

    // Node 1 → neighbor: 2
    nodes[1].nodeId = "1";
    nodes[1].location = {2.0f, 2.0f};
    nodes[1].outboundAccessibleNodes = {"2"};

    // Node 2 → no neighbors
    nodes[2].nodeId = "2";
    nodes[2].location = {3.0f, 3.0f};

    // --- Step 2: Load graph
    Graph graph(nodes);

    // --- Step 3: Build original → new index mapping
    unordered_map<size_t, size_t> originalToNew;
    for (size_t originalIdx = 0; originalIdx < nodes.size(); ++originalIdx) {
        size_t newIdx = graph._vertices.getNewIndex(originalIdx);
        originalToNew[originalIdx] = newIdx;
    }

    // --- Step 4: Expected neighbor map (translated to new indices)
    vector<unordered_set<size_t>> expectedNeighbors(nodes.size());
    expectedNeighbors[0] = {originalToNew[1], originalToNew[2]};
    expectedNeighbors[1] = {originalToNew[2]};
    expectedNeighbors[2] = {};

    // --- Step 5: Validate neighbors
    for (size_t originalIdx = 0; originalIdx < nodes.size(); ++originalIdx) {
        size_t newIdx = originalToNew[originalIdx];

        auto [values, neighbors] = graph._edges.valuesInRow(newIdx);

        unordered_set<size_t> actualNeighbors;
        for (size_t i = 0; i < neighbors.num_items; ++i) {
            actualNeighbors.insert(neighbors.items[i]);
        }

        REQUIRE(actualNeighbors == expectedNeighbors[originalIdx]);
    }
}
