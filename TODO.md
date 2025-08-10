- Create a helper function to compute the haversine formula and update distance calculation/heuristic to use it
- Implement a way to go back and forth with the OpenStreetMap node id
- Test heuristics other than Manhattan distance to see if we get better performance
- Build a priority queue of nodes to actually handle the search (needs log N performance on insert and constant on remove)

Midway airport rough location: "lat": 41.7930767,
      "lng": -87.7412389
O'Hare airport rough location: 
      "lat": 41.982779,
      "lng": -87.805343