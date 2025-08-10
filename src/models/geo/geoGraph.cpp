#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "geoGraph.h"

using geo::distance;
using geo::Graph;
using geo::GraphNode;
using linalg::CsrMatrix;
using linalg::MutableCsrMatrix;
using nlohmann::json;
using std::greater;
using std::cout;
using std::endl;
using std::ifstream;
using std::nullopt;
using std::optional;
using std::pair;
using std::priority_queue;
using std::runtime_error;
using std::string;
using std::to_string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

using PriorityNode = pair<float, size_t>; // priority -> node

Graph::Graph(const string &filename) {
    json j = loadFileAsJson(filename);
    vector<GraphNode> nodes = parseGraphNodes(j);
    loadGraphFromNodes(nodes);
}

Graph::Graph(vector<GraphNode> nodes) {
    loadGraphFromNodes(nodes);
}

optional<vector<GraphNode>> Graph::generateRoute(
    pair<float, float> origin,
    pair<float, float> dest
) {
    // Step 1: Map coordinates to closest nodes
    size_t nodeOrigin = _vertices.approximateNearestPoint(origin.first, origin.second);
    size_t nodeDest = _vertices.approximateNearestPoint(dest.first, dest.second);

    // Step 2: Set up data structures
    using NodeID = size_t;
    unordered_map<NodeID, float> costSoFar;         // Distance from origin
    unordered_map<NodeID, NodeID> cameFrom;         // For path reconstruction
    unordered_set<NodeID> visited;

    // Priority queue: (priority, node)
    priority_queue<PriorityNode, vector<PriorityNode>, greater<>> nodesToVisit;
    nodesToVisit.emplace(distance(_vertices[nodeOrigin], _vertices[nodeDest]), nodeOrigin);

    costSoFar[nodeOrigin] = 0.0f;
    cameFrom[nodeOrigin] = nodeOrigin;

    while (!nodesToVisit.empty()) {
        PriorityNode current = nodesToVisit.top();
        nodesToVisit.pop();

        NodeID currentNode = current.second;

        if (currentNode == nodeDest) {
            // Reconstruct path
            vector<GraphNode> path;
            NodeID node = nodeDest;

            while (node != nodeOrigin) {
                const auto& baseNode = _vertices[node];
                path.push_back(GraphNode{
                    baseNode,
                    "TODO_nodeId", // TODO: set real node ID
                    {"TODO_outbound"} // TODO: populate actual outbound accessible nodes
                });
                node = cameFrom[node];
            }

            const auto& originNode = _vertices[nodeOrigin];
            path.push_back(GraphNode{
                originNode,
                "TODO_nodeId", // TODO: set real node ID
                {"TODO_outbound"} // TODO: populate actual outbound accessible nodes
            });

            reverse(path.begin(), path.end());
            return path;
        }

        if (visited.contains(currentNode)) {
            continue;
        }
        visited.insert(currentNode);

        auto [distances, neighbors] = _edges.valuesInRow(currentNode);

        for (size_t i = 0; i < distances.num_items; ++i) {
            NodeID neighbor = neighbors.items[i];
            float edgeCost = distances.items[i];
            float newCost = costSoFar[currentNode] + edgeCost;

            if (!costSoFar.contains(neighbor) || newCost < costSoFar[neighbor]) {
                costSoFar[neighbor] = newCost;
                float priority = newCost + distance(_vertices[neighbor], _vertices[nodeDest]); // A* priority = cost + heuristic
                nodesToVisit.emplace(priority, neighbor);
                cameFrom[neighbor] = currentNode;
            }
        }
    }

    // If we reach here, no path was found
    return nullopt;
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
            size_t departNodeId = _vertices.getNewIndex(i);
            size_t arriveNodeId = _vertices.getNewIndex(nodeIdToIndex[connectedVertex]);
            edge_construction.addEntry(departNodeId, arriveNodeId, 1); // TODO: Replace 1 with transit time in millis
        }
    }

    _edges = CsrMatrix(edge_construction);
}
