#pragma once

#include <algorithm>
#include <cmath>
#include <limits>
#include <utility>
#include <vector>

#include "models/util/listWithSize.h"

using std::pair;
using std::vector;
using utils::ListWithSize;

namespace utils {
    class Dim2Tree {
    public:
        // Constructor that builds a KD-Tree from a vector of 2D points
        Dim2Tree(vector<pair<float, float>> points);

        // Find the approximate nearest point to the given coordinates
        // Returns the index of the nearest point in the original input vector
        size_t approximateNearestPoint(float latitude, float longitude);

        pair<float, float> operator[](size_t index) const;

        // Get the new index of a point in the input vector
        size_t getNewIndex(size_t nodeIndex) const;

    private:
        // Flat array representation of the KD-Tree
        // Format: [point0_lat, point0_lon, point1_lat, point1_lon, ...]
        ListWithSize<float> _points = ListWithSize<float>(0);
        
        // Metadata for each node in the tree
        // Format: [original_index, split_dimension, ...]
        ListWithSize<size_t> _metadata = ListWithSize<size_t>(0);
        
        // Helper method to build the KD-Tree recursively
        // start and end define the range of points to consider
        // depth determines the splitting dimension (depth % 2)
        // nodeIndex is the index of the current node in the flat array
        // Returns the number of nodes in the subtree
        size_t buildTree(vector<pair<float, float>>& points, 
                         vector<size_t>& indices,
                         size_t start, size_t end, 
                         size_t depth, size_t nodeIndex);

        size_t computeLeftSubtreeSize(size_t currentNodeIndex);
        
        // Helper method for nearest neighbor search
        // Returns the index of the nearest point in the original input vector
        size_t searchNearest(size_t nodeIndex, float lat, float lon, 
                            size_t& bestIndex, float& bestDist) const;
        
        // Calculate squared Euclidean distance between two points
        float distanceSquared(float lat1, float lon1, float lat2, float lon2) const;
        
        // Get the splitting dimension of a node (0 for latitude, 1 for longitude)
        int getSplitDimension(size_t nodeIndex) const;
        
        // Get the left child index of a node
        size_t leftChild(size_t nodeIndex) const;
        
        // Get the right child index of a node
        size_t rightChild(size_t nodeIndex) const;
    };
}
