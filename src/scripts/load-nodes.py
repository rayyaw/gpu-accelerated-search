import osmnx as ox
import json

# Load graph for driving network in Chicago
G = ox.graph_from_place("Chicago, Illinois, USA", network_type='drive')
print("Loaded graph.")

# Build node data
node_data = []

for node_id, data in G.nodes(data=True):
    lat = data['y']
    lng = data['x']

    # Get accessible (neighboring) node IDs from outgoing edges
    neighbors = set()
    for _, neighbor_id, _ in G.out_edges(node_id, keys=True):
        neighbors.add(neighbor_id)

    node_data.append({
        "node_id": node_id,
        "location": {"lat": lat, "lng": lng},
        "neighbors": list(neighbors)
    })

# Save to JSON
with open("data/nodes.json", "w") as f:
    json.dump({"nodes": node_data}, f, indent=2)

print("Saved to data/nodes.json")
