#pragma once

#include <set>
#include <string>
#include <utility>

using std::pair;
using std::set;
using std::string;

namespace geo {
    struct GraphNode {
        pair<float, float> location;

        string nodeId;
        set<string> outboundAccessibleNodes;
    };

    inline float distance(
        const pair<float, float> &loc1,
        const pair<float, float> &loc2
    ) {
        float latDiff = loc1.first - loc2.first;
        float lonDiff = loc1.second - loc2.second;

        return (latDiff + lonDiff) * 1000000.0f;
    }
}