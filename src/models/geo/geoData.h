#pragma once

#include <numbers>
#include <set>
#include <string>
#include <utility>
#include <vector>

using std::numbers::pi;
using std::pair;
using std::set;
using std::string;
using std::vector;

namespace geo {
    struct GraphNode {
        pair<float, float> location;

        string nodeId;
        vector<pair<string, float>> outboundAccessibleNodesWithTime;
    };

    inline float convertToRadians(const float angle) {
        return angle * (pi / 180);
    }

    // Modified from https://github.com/AhiyaHiya/haversine/blob/master/source/haversine.cpp
    inline float distance(
        const pair<float, float> &loc1,
        const pair<float, float> &loc2
    ) {
        const float earths_radius = 6371.0f;

        const float lat_delta = convertToRadians(loc2.first - loc1.first);
        const float lon_delta = convertToRadians(loc2.second - loc2.second);

        const float converted_lat1 = convertToRadians(loc1.first);
        const float converted_lat2 = convertToRadians(loc2.first);

        const float a =
            pow(sin(lat_delta / 2), 2) + cos(converted_lat1) * cos(converted_lat2) * pow(sin(lon_delta / 2), 2);

        const float c = 2 * atan2(sqrt(a), sqrt(1 - a));
        const float d = earths_radius * c;

        // https://en.wikipedia.org/wiki/Texas_State_Highway_130
        // = 85 mph
        float highestRoadLegalSpeedKps = 136.7942 / 60 / 60;

        return d / highestRoadLegalSpeedKps;
    }

    // convert our passed value to radians_t
    
}