#include <iostream>

#include "dim2Tree.h"

using std::cout;
using std::endl;
using utils::Dim2Tree;
using utils::ListWithSize;

Dim2Tree::Dim2Tree(vector<pair<float, float>> points) {
    // Initialize the tree with the appropriate size
    // We need 2 floats per point (lat, lon)
    _points = ListWithSize<float>(points.size() * 2);
    
    // We need 2 ints per node (original index, split dimension)
    _metadata = ListWithSize<size_t>(points.size() * 2);
    
    // Create a vector of indices to track the original positions
    vector<size_t> indices(points.size());
    for (size_t i = 0; i < points.size(); i++) {
        indices[i] = i;
    }
    
    // Build the KD-Tree recursively
    buildTree(points, indices, 0, points.size(), 0, 0);
}

size_t Dim2Tree::buildTree(vector<pair<float, float>>& points, 
                          vector<size_t>& indices,
                          size_t start, size_t end, 
                          size_t depth, size_t nodeIndex) {

    if (start >= end) {
        return 0; // Empty subtree
    }
    
    // Choose axis based on depth (cycle through dimensions)
    int axis = depth % 2;
    
    // Sort points along the chosen axis
    std::sort(indices.begin() + start, indices.begin() + end,
              [&points, axis](size_t a, size_t b) {
                  if (axis == 0) {
                      return points[a].second < points[b].second;
                  } else {
                      return points[a].first < points[b].first;
                  }
              });
    
    // Find median
    size_t medianIdx = start + (end - start) / 2;
    size_t originalIndex = indices[medianIdx];
    
    // Store the point and metadata
    _points[nodeIndex * 2] = points[originalIndex].first;      // Latitude
    _points[nodeIndex * 2 + 1] = points[originalIndex].second; // Longitude
    _metadata[nodeIndex * 2] = originalIndex;                  // Original index
    _metadata[nodeIndex * 2 + 1] = axis;                       // Split dimension
    
    // Recursively build subtrees
    size_t leftNodes = 0;
    size_t rightNodes = 0;
    
    // Build left subtree if there are points to the left of median
    if (start < medianIdx) {
        leftNodes = buildTree(points, indices, start, medianIdx, depth + 1, leftChild(nodeIndex));
    }
    
    // Build right subtree if there are points to the right of median
    if (medianIdx + 1 < end) {
        rightNodes = buildTree(points, indices, medianIdx + 1, end, depth + 1, rightChild(nodeIndex));
    }
    
    // Return total number of nodes in this subtree (including this node)
    return 1 + leftNodes + rightNodes;
}

size_t Dim2Tree::approximateNearestPoint(float latitude, float longitude) {
    // Initialize best distance to infinity and best index to invalid
    float bestDist = std::numeric_limits<float>::max();
    size_t bestIndex = 0;
    
    // Start the search from the root node (index 0)
    searchNearest(0, latitude, longitude, bestIndex, bestDist);
    
    return bestIndex;
}

pair<float, float> Dim2Tree::operator[](size_t index) const {
    if (index >= _points.size()) {
        throw out_of_range(format("Attempt to access element {} in Dim2Tree of size {}", index, _points.size() / 2));
    }

    return pair<float, float>(_points[index * 2], _points[index * 2 + 1]);
}

size_t Dim2Tree::searchNearest(size_t nodeIndex, float lat, float lon, 
                              size_t& bestIndex, float& bestDist) const {
    // If we've gone beyond the tree size, return
    if (nodeIndex >= _points.size() / 2) {
        return bestIndex;
    }
    
    // Get the current node's point
    float nodeLat = _points[nodeIndex * 2];
    float nodeLon = _points[nodeIndex * 2 + 1];
    
    // Calculate distance to the current node
    float dist = distanceSquared(lat, lon, nodeLat, nodeLon);
    
    // Update best distance if this node is closer
    if (dist < bestDist) {
        bestDist = dist;
        bestIndex = getOriginalIndex(nodeIndex);
    }
    
    // Get the splitting dimension for this node
    int splitDim = getSplitDimension(nodeIndex);
    
    // Determine which child to search first based on the splitting dimension
    float targetValue = (splitDim == 0) ? lat : lon;
    float nodeValue = (splitDim == 0) ? nodeLat : nodeLon;
    
    size_t firstChild, secondChild;
    if (targetValue < nodeValue) {
        firstChild = leftChild(nodeIndex);
        secondChild = rightChild(nodeIndex);
    } else {
        firstChild = rightChild(nodeIndex);
        secondChild = leftChild(nodeIndex);
    }
    
    // Search the first subtree
    searchNearest(firstChild, lat, lon, bestIndex, bestDist);
    
    // Calculate the distance to the splitting plane
    float planeDist = targetValue - nodeValue;
    planeDist = planeDist * planeDist; // Square it
    
    // If the distance to the plane is less than the current best distance,
    // we need to check the other subtree as well
    if (planeDist < bestDist) {
        searchNearest(secondChild, lat, lon, bestIndex, bestDist);
    }
    
    return bestIndex;
}

float Dim2Tree::distanceSquared(float lat1, float lon1, float lat2, float lon2) const {
    // Simple Euclidean distance squared
    float dLat = lat1 - lat2;
    float dLon = lon1 - lon2;
    return dLat * dLat + dLon * dLon;
}

size_t Dim2Tree::getOriginalIndex(size_t nodeIndex) const {
    return _metadata[nodeIndex * 2];
}

int Dim2Tree::getSplitDimension(size_t nodeIndex) const {
    return _metadata[nodeIndex * 2 + 1];
}

size_t Dim2Tree::leftChild(size_t nodeIndex) const {
    return 2 * nodeIndex + 1;
}

size_t Dim2Tree::rightChild(size_t nodeIndex) const {
    return 2 * nodeIndex + 2;
}
