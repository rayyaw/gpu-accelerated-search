from pyrosm import OSM
import osmnx as ox
import json

# Load graph for driving network
# Load your .osm.pbf file
osm = OSM("data/north-america-latest.osm.pbf")
nodes, edges = osm.get_network(network_type="driving")

# Convert to a NetworkX graph
G = osm.to_graph(nodes, edges, graph_type="networkx")

G = ox.add_edge_speeds(G)
G = ox.add_edge_travel_times(G)
print("Loaded graph.")

# Build node data
node_data = []

for node_id, data in G.nodes(data=True):
    lat = data['y']
    lng = data['x']

    # Get accessible (neighboring) node IDs from outgoing edges
    neighbors = []
    for _, neighbor_id, edge_data in G.out_edges(node_id, data=True):
        if node_id == neighbor_id:
            continue  # Skip self-loops

        travel_time = edge_data.get('travel_time', 1.0)  # Fallback to 1 sec if missing
        neighbors.append({
            "node_id": neighbor_id,
            "transit_time_sec": round(travel_time, 2) if travel_time else 1.0
        })

    node_data.append({
        "node_id": node_id,
        "location": {"lat": lat, "lng": lng},
        "neighbors": neighbors
    })

# Save to JSON
with open("data/nodes.json", "w") as f:
    json.dump({"nodes": node_data}, f, indent=2)

print("Saved to data/nodes.json")
