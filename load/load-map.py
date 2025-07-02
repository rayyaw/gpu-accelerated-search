import osmnx as ox
import json

# Get the graph
G = ox.graph_from_place("Chicago, Illinois, USA", network_type='drive')
print("Loaded graph.")

# Build a lookup for node coordinates
node_coords = {node: (data['y'], data['x']) for node, data in G.nodes(data=True)}

# Extract segment data
segment_data = []

for u, v, key, data in G.edges(keys=True, data=True):
    segment_id = f"{u}-{v}-{key}"

    # Find connecting segments: any edge starting or ending at u or v
    connected_ids = set()
    for neighbor in G.edges(u, keys=True):
        if neighbor != (u, v, key):
            connected_ids.add(f"{neighbor[0]}-{neighbor[1]}-{neighbor[2]}")
    for neighbor in G.edges(v, keys=True):
        if neighbor != (u, v, key):
            connected_ids.add(f"{neighbor[0]}-{neighbor[1]}-{neighbor[2]}")

    # Get start and end locations
    start_latlng = {"lat": node_coords[u][0], "lng": node_coords[u][1]}
    end_latlng = {"lat": node_coords[v][0], "lng": node_coords[v][1]}

    segment_data.append({
        "segment_id": segment_id,
        "connected_segments": list(connected_ids),
        "start_location": start_latlng,
        "end_location": end_latlng
    })

# Save to JSON
with open("data/segments.json", "w") as f:
    json.dump(segment_data, f, indent=2)

print("Saved to data/segments.json")
