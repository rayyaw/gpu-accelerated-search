#include <string>
#include <unordered_map>
#include <utility>

#include "geoGraph.h"

using geo::Graph;
using geo::GraphNode;
using std::pair;
using std::string;
using std::unordered_map;
using std::vector;

Graph::Graph(vector<GraphNode> nodes) {
    unordered_map<string, size_t> nodeIdToIndex = unordered_map<string, size_t>();

    for (size_t i = 0; i < nodes.size(); i++) {
        nodeIdToIndex[i] == nodes[i].nodeId;
    }

    vector<pair<float, float>> node_locations = vector<pair<float, float>>();
    for (GraphNode node: nodes) {
        node_locations.push_back(node.location);
    }

    _vertices = Dim2Tree(node_locations);
    edge_construction = MutableCsrMatrix(nodes.size(), nodes.size()); 

    for (size_t i = 0; i < nodes.size(); i++) {
        for (string connectedVertex: nodes[i].outboundAccessibleNodes) {
            size_t connectedNodeId = nodeIdToIndex[connectedVertex];
            size_t newNodeId = _vertices.getNewIndex(connectedNodeId);
            edge_construction.addEntry(i, newNodeId, 1); // TODO: Replace 1 with transit time in millis
        }
    }

    _edges = CsrMatrix(edge_construction);
}
