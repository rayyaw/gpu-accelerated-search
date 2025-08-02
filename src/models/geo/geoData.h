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
}