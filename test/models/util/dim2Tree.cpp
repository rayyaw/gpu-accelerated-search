#include <cmath>
#include <iostream>
#include <vector>
#include <utility>

#include "catch/catch.hpp"
#include "models/util/dim2Tree.h"

using std::cout;
using std::endl;
using std::pair;
using std::vector;
using utils::Dim2Tree;

// Helper function to create a test tree with a simple set of points
Dim2Tree createTestTree() {
    vector<pair<float, float>> points = {
        {2.0f, 3.0f},   // 0
        {5.0f, 4.0f},   // 1
        {9.0f, 6.0f},   // 2
        {4.0f, 7.0f},   // 3
        {8.0f, 1.0f},   // 4
        {7.0f, 2.0f}    // 5
    };
    return Dim2Tree(points);
}

void validateApproximateNearestPoint(
    Dim2Tree tree,
    float inputLat, float inputLon,
    float expectedLat, float expectedLon
) {
    size_t approximateNearestPoint = tree.approximateNearestPoint(inputLat, inputLon);
    pair<float, float> approximateLocation = tree[approximateNearestPoint];
    
    REQUIRE(approximateLocation.first == expectedLat);
    REQUIRE(approximateLocation.second == expectedLon);
}

TEST_CASE("Dim2Tree construction", "[Dim2Tree]") {
    // Test that the tree can be constructed without errors
    REQUIRE_NOTHROW(createTestTree());
}

TEST_CASE("Dim2Tree nearest point search - exact matches", "[Dim2Tree]") {
    Dim2Tree tree = createTestTree();
    
    validateApproximateNearestPoint(
        tree,
        2.0f, 3.0f, // input
        2.0f, 3.0f // output
    );

    validateApproximateNearestPoint(
        tree,
        5.0f, 4.0f, // input
        5.0f, 4.0f // output
    );

    validateApproximateNearestPoint(
        tree,
        9.0f, 6.0f, // input
        9.0f, 6.0f // output
    );

    validateApproximateNearestPoint(
        tree,
        4.0f, 7.0f, // input
        4.0f, 7.0f // output
    );

    validateApproximateNearestPoint(
        tree,
        8.0f, 1.0f, // input
        8.0f, 1.0f // output
    );

    validateApproximateNearestPoint(
        tree,
        7.0f, 2.0f, // input
        7.0f, 2.0f // output
    );
}

TEST_CASE("Dim2Tree nearest point search - approximate matches", "[Dim2Tree]") {
    Dim2Tree tree = createTestTree();
    validateApproximateNearestPoint(
        tree,
        2.1f, 3.1f, // input
        2.0f, 3.0f // output
    );

    validateApproximateNearestPoint(
        tree,
        5.2f, 4.1f, // input
        5.0f, 4.0f // output
    );

    validateApproximateNearestPoint(
        tree,
        8.8f, 5.9f, // input
        9.0f, 6.0f // output
    );

    validateApproximateNearestPoint(
        tree,
        4.2f, 6.8f, // input
        4.0f, 7.0f // output
    );

    validateApproximateNearestPoint(
        tree,
        7.9f, 1.2f, // input
        8.0f, 1.0f // output
    );

    validateApproximateNearestPoint(
        tree,
        6.8f, 2.2f, // input
        7.0f, 2.0f // output
    );
}

TEST_CASE("Dim2Tree nearest point search - distant points", "[Dim2Tree]") {
    Dim2Tree tree = createTestTree();
    
    validateApproximateNearestPoint(
        tree,
        0.0f, 0.0f, // input
        2.0f, 3.0f // output
    );

    validateApproximateNearestPoint(
        tree,
        10.0f, 10.0f, // input
        9.0f, 6.0f // output
    );
}

TEST_CASE("Dim2Tree with single point", "[Dim2Tree]") {
    // Test a tree with only one point
    vector<pair<float, float>> points = {{1.0f, 1.0f}};
    Dim2Tree tree(points);
    
    // Any query should return the only point
    validateApproximateNearestPoint(
        tree,
        0.0f, 0.0f, // input
        1.0f, 1.0f // output
    );

    validateApproximateNearestPoint(
        tree,
        10.0f, 10.0f, // input
        1.0f, 1.0f // output
    );

    validateApproximateNearestPoint(
        tree,
        1.0f, 1.0f, // input
        1.0f, 1.0f // output
    );
}

TEST_CASE("Dim2Tree with many points - power of 2", "[Dim2Tree]") {
    // Create a larger set of points in a grid pattern
    vector<pair<float, float>> points;
    for (float x = 0.0f; x < 8.0f; x += 1.0f) {
        for (float y = 0.0f; y < 8.0f; y += 1.0f) {
            points.push_back({x, y});
        }
    }

    Dim2Tree tree(points);

    validateApproximateNearestPoint(
        tree,
        1.1f, 2.2f, // input
        1.0f, 2.0f // output
    );
}

TEST_CASE("Dim2Tree with many points - not a power of 2", "[Dim2Tree]") {
    // Create a larger set of points in a grid pattern
    vector<pair<float, float>> points;
    for (float x = 0.0f; x < 10.0f; x += 1.0f) {
        for (float y = 0.0f; y < 10.0f; y += 1.0f) {
            points.push_back({x, y});
        }
    }

    Dim2Tree tree(points);
    
    validateApproximateNearestPoint(
        tree,
        1.1f, 2.2f, // input
        1.0f, 3.0f // output
    );
    
    validateApproximateNearestPoint(
        tree,
        5.6f, 7.8f, // input
        5.0f, 8.0f // output
    );
}
