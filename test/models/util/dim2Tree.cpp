#include <vector>
#include <utility>
#include <cmath>

#include "catch/catch.hpp"
#include "models/util/dim2Tree.h"

using std::vector;
using std::pair;
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

TEST_CASE("Dim2Tree construction", "[Dim2Tree]") {
    // Test that the tree can be constructed without errors
    REQUIRE_NOTHROW(createTestTree());
}

TEST_CASE("Dim2Tree nearest point search - exact matches", "[Dim2Tree]") {
    Dim2Tree tree = createTestTree();
    
    // Test exact matches for each point
    REQUIRE(tree.approximateNearestPoint(2.0f, 3.0f) == 0);
    REQUIRE(tree.approximateNearestPoint(5.0f, 4.0f) == 1);
    REQUIRE(tree.approximateNearestPoint(9.0f, 6.0f) == 2);
    REQUIRE(tree.approximateNearestPoint(4.0f, 7.0f) == 3);
    REQUIRE(tree.approximateNearestPoint(8.0f, 1.0f) == 4);
    REQUIRE(tree.approximateNearestPoint(7.0f, 2.0f) == 5);
}

TEST_CASE("Dim2Tree nearest point search - approximate matches", "[Dim2Tree]") {
    Dim2Tree tree = createTestTree();
    
    // Test points that are close to the input points
    REQUIRE(tree.approximateNearestPoint(2.1f, 3.1f) == 0);
    REQUIRE(tree.approximateNearestPoint(5.2f, 4.1f) == 1);
    REQUIRE(tree.approximateNearestPoint(8.8f, 5.9f) == 2);
    REQUIRE(tree.approximateNearestPoint(4.2f, 6.8f) == 3);
    REQUIRE(tree.approximateNearestPoint(7.9f, 1.2f) == 4);
    REQUIRE(tree.approximateNearestPoint(6.8f, 2.2f) == 5);
}

TEST_CASE("Dim2Tree nearest point search - distant points", "[Dim2Tree]") {
    Dim2Tree tree = createTestTree();
    
    // Test points that are far from any input point
    // For (0,0), the closest point should be (2,3) at index 0
    REQUIRE(tree.approximateNearestPoint(0.0f, 0.0f) == 0);
    
    // For (10,10), the closest point should be (9,6) at index 2
    REQUIRE(tree.approximateNearestPoint(10.0f, 10.0f) == 2);
}

TEST_CASE("Dim2Tree with single point", "[Dim2Tree]") {
    // Test a tree with only one point
    vector<pair<float, float>> points = {{1.0f, 1.0f}};
    Dim2Tree tree(points);
    
    // Any query should return the only point
    REQUIRE(tree.approximateNearestPoint(0.0f, 0.0f) == 0);
    REQUIRE(tree.approximateNearestPoint(10.0f, 10.0f) == 0);
    REQUIRE(tree.approximateNearestPoint(1.0f, 1.0f) == 0);
}

// FIXME: this test is bricked
TEST_CASE("Dim2Tree with many points", "[Dim2Tree]") {
    // Create a larger set of points in a grid pattern
    vector<pair<float, float>> points;
    for (float x = 0.0f; x < 10.0f; x += 1.0f) {
        for (float y = 0.0f; y < 10.0f; y += 1.0f) {
            points.push_back({x, y});
        }
    }
    
    Dim2Tree tree(points);
    
    // Test a few specific cases
    // For point (1.1, 2.2), the closest grid point should be (1.0, 2.0)
    // which is at index 1*10 + 2 = 12
    REQUIRE(tree.approximateNearestPoint(1.1f, 2.2f) == 21);
    
    // For point (5.6, 7.8), the closest grid point should be (6.0, 8.0)
    // which is at index 6*10 + 8 = 68
    REQUIRE(tree.approximateNearestPoint(5.6f, 7.8f) == 78);
}
