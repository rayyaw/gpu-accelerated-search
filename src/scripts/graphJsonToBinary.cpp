#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

#include "models/geo/geoData.h"
#include "models/geo/geoGraph.h"

using geo::Graph;
using geo::GraphNode;
using std::cout;
using std::endl;
using std::ofstream;
using std::optional;
using std::vector;

int main() {
    Graph graph = Graph("data/nodes.json");

    ofstream output("data/nodes.bin", std::ios::binary);
    output << graph;
    output.close();

    return 0;
}
