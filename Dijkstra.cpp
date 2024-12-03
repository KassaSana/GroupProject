
// Dijkstra's Algorithm
vector<int> dijkstra(const unordered_map<int, vector<pair<int, double>>> &graph, int start, int end) {

    unordered_map<int, double> dist; // keeps track of the shortest distance node to node
    unordered_map<int, int> original; //for remaking graph

    priority_queue<Node, vector<Node>, greater<Node>> pq; //next node with most short distance

    for (auto it = graph.begin(); it != graph.end(); ++it) {
        dist[it->first] = numeric_limits<double>::infinity();
    }

    dist[start] = 0.0;

    pq.push({start, 0.0});
    while (!pq.empty()) {
        int current = pq.top().id;//gets node w/ smallest distance
        pq.pop();

        if (current == end) break; //if done stop

        //goes through all neighbors of current node
        for (const auto &[neighbor, weight] : graph.at(current)) {
            double newDist = dist[current] + weight;
            if (newDist < dist[neighbor]) {
                dist[neighbor] = newDist;
                original[neighbor] = current;
                pq.push({neighbor, newDist});
            }
        }
    }

    // remake path
    vector<int> path;
    for (int at = end; at != start; at = original[at]) {
        path.push_back(at);
    }
    path.push_back(start);
    reverse(path.begin(), path.end());
    return path;
}
