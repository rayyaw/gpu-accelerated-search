// graphJsonToBinary.cpp
// Take in the GeoGraph as a .json file, and output it into a binary object for faster usage

#include "models/geo/geoGraph.h"

using geo::Graph;

int main() {
    Graph graph = Graph("data/nodes.json");

    return 0;
}
