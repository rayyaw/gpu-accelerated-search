#include <fstream>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "data_io.h"
#include "external/nlohmann/json.hpp"

using linalg::CsrMatrix;
using linalg::MutableCsrMatrix;
using nlohmann::detail::parse_error;
using nlohmann::json;
using std::cout;
using std::endl;
using std::ifstream;
using std::runtime_error;
using std::size_t;
using std::string;
using std::unordered_map;
using std::vector;

CsrMatrix data_io::loadSegmentsMatrix(const string &filename) {
    // Open the JSON file
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Failed to open JSON file: " + filename);
    }

    // Parse JSON data
    json data;
    try {
        file >> data;
    } catch (const parse_error& e) {
        file.close();
        throw runtime_error("JSON parse error: " + string(e.what()));
    }
    file.close();

    if (!data.is_array()) {
        throw runtime_error("JSON data must be an array of segments");
    }

    // Map segment IDs to matrix indices
    std::unordered_map<std::string, size_t> segmentToIndex;
    size_t index = 0;
    size_t totalSegments = data.size();
    size_t progressCounter = 0;
    for (const auto& segment : data) {
        if (segment.contains("segment_id") && segment["segment_id"].is_string()) {
            std::string id = segment["segment_id"].get<std::string>();
            if (segmentToIndex.find(id) == segmentToIndex.end()) {
                segmentToIndex[id] = index++;
            }
        }
        progressCounter++;
        if (progressCounter % 10000 == 0) {
            cout << "Processed " << progressCounter << " of " << totalSegments << " segments for ID mapping." << endl;
        }
    }
    if (progressCounter % 10000 != 0) {
        cout << "Processed " << progressCounter << " of " << totalSegments << " segments for ID mapping." << endl;
    }

    size_t numSegments = segmentToIndex.size();
    if (numSegments == 0) {
        throw runtime_error("No segments found in JSON data");
    }

    // Use MutableCsrMatrix to build the adjacency matrix
    MutableCsrMatrix matrix(numSegments, numSegments);

    // Populate the matrix based on connected segments
    size_t processedSegments = 0;
    size_t totalSegmentsForMatrix = data.size();
    for (const auto& segment : data) {
        if (segment.contains("segment_id") && segment["segment_id"].is_string() &&
            segment.contains("connected_segments") && segment["connected_segments"].is_array()) {
            std::string id = segment["segment_id"].get<std::string>();
            auto it = segmentToIndex.find(id);
            if (it == segmentToIndex.end()) {
                continue; // Should not happen if mapping is correct
            }
            size_t row = it->second;

            for (const auto& connectedId : segment["connected_segments"]) {
                if (connectedId.is_string()) {
                    std::string connId = connectedId.get<std::string>();
                    auto connIt = segmentToIndex.find(connId);
                    if (connIt != segmentToIndex.end()) {
                        size_t col = connIt->second;
                        matrix.addEntry(row, col, 1); // Use 1 to indicate connection
                    }
                }
            }
        }
        processedSegments++;
        if (processedSegments % 10000 == 0) {
            cout << "Processed " << processedSegments << " of " << totalSegmentsForMatrix << " segments for matrix population." << endl;
        }
    }
    if (processedSegments % 10000 != 0) {
        cout << "Processed " << processedSegments << " of " << totalSegmentsForMatrix << " segments for matrix population." << endl;
    }

    // Convert MutableCsrMatrix to CsrMatrix using the dedicated constructor
    return CsrMatrix(matrix);
}
