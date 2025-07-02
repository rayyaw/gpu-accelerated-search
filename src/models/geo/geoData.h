#pragma once

#include <set>
#include <string>

using std::set;
using std::string;

namespace geo {
    struct Point {
        float lat, float lng;
    }

    struct GraphEdge {
        Point start;
        Point end;

        string openStreetMapId;
        set<string> nearbyIds;
    }
}