#include <iostream>
#include <string>
#include <stdexcept>

#include "gpu/gpu.h"
#include "data_io/data_io.h"

using std::cout;
using std::endl;
using std::string;
using std::stod;
using std::runtime_error;

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cout << "Usage: " << argv[0] << " <origin_lat> <origin_lon> <dest_lat> <dest_lon>" << endl;
        cout << "  origin_lat: Latitude of the origin point" << endl;
        cout << "  origin_lon: Longitude of the origin point" << endl;
        cout << "  dest_lat: Latitude of the destination point" << endl;
        cout << "  dest_lon: Longitude of the destination point" << endl;
        return 1;
    }

    try {
        double origin_lat = stod(argv[1]);
        double origin_lon = stod(argv[2]);
        double dest_lat = stod(argv[3]);
        double dest_lon = stod(argv[4]);

        cout << "Origin: (" << origin_lat << ", " << origin_lon << ")" << endl;
        cout << "Destination: (" << dest_lat << ", " << dest_lon << ")" << endl;

        // Load the graph from segments.json into a CSR matrix
        string filename = "data/segments.json";
        linalg::CsrMatrix graph = data_io::loadSegmentsMatrix(filename);
        cout << "Graph loaded from " << filename << " into CSR matrix." << endl;

        // Calculate and print the size of the graph in MB
        size_t memory_size_bytes = graph.getMemorySize();
        double size_mb = static_cast<double>(memory_size_bytes) / (1024.0 * 1024.0);
        cout << "Graph size: " << size_mb << " MB" << endl;

        // TODO: Implement logic to find nearest segments or nodes for origin and destination
        // and perform pathfinding using the CSR matrix.

    } catch (const runtime_error& e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    } catch (const std::invalid_argument& e) {
        cout << "Error: Invalid argument for latitude or longitude. Please provide valid numbers." << endl;
        return 1;
    } catch (const std::out_of_range& e) {
        cout << "Error: Latitude or longitude value out of range." << endl;
        return 1;
    }

    return 0;
}
