#include <vector>

#include "catch/catch.hpp"

#include "models/util/listWithSize.h"

using std::vector;
using utils::ListWithSize;

ListWithSize<int> makeList() {
    int exampleArray[5] = {4, 5, 6, 7, 8};
    return ListWithSize<int>(5, exampleArray);
}

ListWithSize<int> *makeListPtr() {
    int exampleArray[5] = {4, 5, 6, 7, 8};
    return new ListWithSize<int>(5, exampleArray);
}

TEST_CASE("Standard ListWithSize initialization and access", "[ListWithSize]") {
    ListWithSize<int> exampleList = makeList();

    REQUIRE(exampleList.size() == 5);
    REQUIRE(exampleList[0] == 4);
    REQUIRE(exampleList[1] == 5);
    REQUIRE(exampleList[2] == 6);
    REQUIRE(exampleList[3] == 7);
    REQUIRE(exampleList[4] == 8);
}

TEST_CASE("ListWithSize vector constructor", "[ListWithSize]") {
    vector<int> vec = vector<int>();
    for (int i = 4; i < 9; i++) {
        vec.push_back(i);
    }

    ListWithSize<int> exampleList(vec);

    REQUIRE(exampleList.size() == 5);
    REQUIRE(exampleList[0] == 4);
    REQUIRE(exampleList[1] == 5);
    REQUIRE(exampleList[2] == 6);
    REQUIRE(exampleList[3] == 7);
    REQUIRE(exampleList[4] == 8);
}

TEST_CASE("ListWithSize copy constructor", "[ListWithSize]") {
    ListWithSize<int> *original = makeListPtr();
    ListWithSize<int> copyConstructed = ListWithSize<int>(*original);
    delete original;

    REQUIRE(copyConstructed.size() == 5);
    REQUIRE(copyConstructed[0] == 4);
    REQUIRE(copyConstructed[1] == 5);
    REQUIRE(copyConstructed[2] == 6);
    REQUIRE(copyConstructed[3] == 7);
    REQUIRE(copyConstructed[4] == 8);
}

TEST_CASE("ListWithSize copy assignment operator", "[ListWithSize]") {
    ListWithSize<int> *original = makeListPtr();
    ListWithSize<int> copyAssigned = *original;
    delete original;

    REQUIRE(copyAssigned.size() == 5);
    REQUIRE(copyAssigned[0] == 4);
    REQUIRE(copyAssigned[1] == 5);
    REQUIRE(copyAssigned[2] == 6);
    REQUIRE(copyAssigned[3] == 7);
    REQUIRE(copyAssigned[4] == 8);
}

// TODO: Copy and move constructor tests to ensure the memory is reallocated