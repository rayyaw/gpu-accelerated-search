#include <fstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

#include "geoGraph.h"

using geo::Graph;
using geo::GraphNode;
using linalg::CsrMatrix;
using linalg::MutableCsrMatrix;
using nlohmann::json;
using std::ifstream;
using std::pair;
using std::runtime_error;
using std::string;
using std::to_string;
using std::unordered_map;
using std::vector;

Graph::Graph(const string &filename) {
    json j = loadFileAsJson(filename);
    vector<GraphNode> nodes = parseGraphNodes(j);
    loadGraphFromNodes(nodes);
}

Graph::Graph(vector<GraphNode> nodes) {
    loadGraphFromNodes(nodes);
}

json Graph::loadFileAsJson(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Failed to open file: " + filename);
    }

    json parsedJson;
    file >> parsedJson;

    return parsedJson;
}

// Function to parse JSON into a vector of GraphNodes
vector<GraphNode> Graph::parseGraphNodes(const json& j) {
    vector<GraphNode> graphNodes;

    if (!j.contains("nodes") || !j["nodes"].is_array()) {
        throw runtime_error("Invalid or missing 'nodes' array in JSON.");
    }

    for (const auto& nodeJson : j["nodes"]) {
        GraphNode node;

        // Parse node ID
        node.nodeId = to_string(nodeJson["node_id"].get<uint64_t>());

        // Parse location
        if (nodeJson.contains("location")) {
            const auto& loc = nodeJson["location"];
            if (loc.contains("lat") && loc.contains("lng")) {
                node.location.first = loc["lat"].get<float>();
                node.location.second = loc["lng"].get<float>();
            }
        }

        // Parse neighbors
        if (nodeJson.contains("neighbors") && nodeJson["neighbors"].is_array()) {
            for (const auto& neighbor : nodeJson["neighbors"]) {
                if (neighbor.is_number()) {
                    node.outboundAccessibleNodes.insert(to_string(neighbor.get<size_t>()));
                }
            }
        }

        graphNodes.push_back(std::move(node));
    }

    return graphNodes;
}

void Graph::loadGraphFromNodes(vector<GraphNode> nodes) {
    unordered_map<string, size_t> nodeIdToIndex = unordered_map<string, size_t>();

    for (size_t i = 0; i < nodes.size(); i++) {
        nodeIdToIndex[nodes[i].nodeId] = i;
    }

    vector<pair<float, float>> node_locations = vector<pair<float, float>>();
    for (GraphNode node: nodes) {
        node_locations.push_back(node.location);
    }

    _vertices = Dim2Tree(node_locations);
    MutableCsrMatrix edge_construction = MutableCsrMatrix(nodes.size(), nodes.size()); 

    for (size_t i = 0; i < nodes.size(); i++) {
        for (string connectedVertex: nodes[i].outboundAccessibleNodes) {
            size_t connectedNodeId = nodeIdToIndex[connectedVertex];
            size_t newNodeId = _vertices.getNewIndex(connectedNodeId);
            edge_construction.addEntry(i, newNodeId, 1); // TODO: Replace 1 with transit time in millis
        }
    }

    _edges = CsrMatrix(edge_construction);
}
