#include <iostream>
#include <optional>
#include <vector>

#include "models/geo/geoData.h"
#include "models/geo/geoGraph.h"

using geo::Graph;
using geo::GraphNode;
using std::cout;
using std::endl;
using std::optional;
using std::vector;

int main() {
    Graph graph = Graph("data/nodes.json");

    optional<vector<GraphNode>> route = graph.generateRoute(
        {41.9837686,-87.850425},
        {41.78420733909216, -87.74131447386281}
    );

    if (route.has_value()) {
            cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        cout << "<gpx version=\"1.1\" creator=\"YourAppName\" "
                "xmlns=\"http://www.topografix.com/GPX/1/1\" "
                "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
                "xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1 "
                "http://www.topografix.com/GPX/1/1/gpx.xsd\">\n";

        cout << "  <trk>\n";
        cout << "    <name>Generated Route</name>\n";
        cout << "    <trkseg>\n";

        for (const auto& node : route.value()) {
            float lat = node.location.first;
            float lon = node.location.second;
            cout << "      <trkpt lat=\"" << lat << "\" lon=\"" << lon << "\" />\n";
        }

        cout << "    </trkseg>\n";
        cout << "  </trk>\n";
        cout << "</gpx>\n";
    } else {
        cout << "No route could be found." << endl;
    }

    return 0;
}
