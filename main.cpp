#include <iostream>
#include <limits>
#include <unordered_map>
#include <cstdlib>
#include <vector>
#include <queue>
#include <cmath>
#include <iomanip>
#include <ctime>
#include <algorithm>
using namespace std;

// Struct to represent a cell in the maze
struct Cell {
    int x; //position of cell in maze
    int y; //position of cell in maze
    int id; // unique identifier for the cell
    string type; // type of cell -> enter, exit, path, wall
};

// Priority queue node for pathfinding
struct Node {
    int id; //node id
    double cost; //cost of traveling node
    bool operator>(const Node &other) const {
        return cost > other.cost; // prioritize smallest cost first
    }
};


unordered_map<int, vector<pair<int, double>>> graphBuild(const vector<Cell> &maze, int rows, int cols) {
    unordered_map<int, vector<pair<int, double>>> graph;

    for (const auto &cell : maze) {
        if (cell.type == "wall") continue;

        // checking through the neighbors
        vector<pair<int, int>> neighbors = {
                {cell.x - 1, cell.y}, // up
                {cell.x + 1, cell.y}, // down
                {cell.x, cell.y - 1}, // left
                {cell.x, cell.y + 1}  // right
        };

        // checks if each neighbor is within bounds (rows and columns)
        for (const auto &[nx, ny] : neighbors) {
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols) {
                int neighborId = nx * cols + ny; // (x,y) -> unique ID
                if (maze[neighborId].type != "wall") {
                    double cost = 1.0; // edge
                    graph[cell.id].emplace_back(neighborId, cost);
                }
            }
        }
    }
    return graph;
}

double euclideanDistance(int x1, int y1, int x2, int y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}


// creatas random maze with given rows and columns
vector<Cell> generateMaze(int rows, int cols) {
    vector<Cell> maze(rows * cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int id = i * cols + j;
            maze[id] = {i, j, id, (rand() % 10 < 2) ? "wall" : "path"};
        }
    }

    maze[0].type = "enter";
    maze[rows * cols - 1].type = "exit";
    return maze;
}


// A* Algorithm
vector<int> aStar(const unordered_map<int, vector<pair<int, double>>> &graph, const vector<Cell> &maze, int start, int end) {

    unordered_map<int, double> gScore, fScore;
    unordered_map<int, int> predecessors;
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    for (const auto &[id, _] : graph) {
        gScore[id] = numeric_limits<double>::infinity();
        fScore[id] = numeric_limits<double>::infinity();
    }

    gScore[start] = 0.0;
    fScore[start] = euclideanDistance(maze[start].x, maze[start].y, maze[end].x, maze[end].y);
    pq.push({start, fScore[start]});

    while (!pq.empty()) {
        int current = pq.top().id;
        pq.pop();

        if (current == end) break; //stops if at end

        for (const auto &[neighbor, weight] : graph.at(current)) {
            double tentativeGScore = gScore[current] + weight;
            if (tentativeGScore < gScore[neighbor]) {
                gScore[neighbor] = tentativeGScore;
                fScore[neighbor] = tentativeGScore + euclideanDistance(maze[neighbor].x, maze[neighbor].y, maze[end].x, maze[end].y);
                predecessors[neighbor] = current;
                pq.push({neighbor, fScore[neighbor]});
            }
        }
    }
    //remake path
    vector<int> path;
    int at = end;
    while (at != start) {
        path.insert(path.begin(), at); // Insert at the beginning
        at = predecessors[at];
    }
    path.insert(path.begin(), start); // Insert the start node
    return path;

}


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

// Display maze w/ path
void dispMaze(const vector<Cell> &maze, const vector<int> &path, int rows, int cols) {
    vector<string> mazeDisplay(rows * cols, ".");
    for (int id : path) {
        mazeDisplay[id] = "*";
    }

    mazeDisplay[0] = "S";  // start
    mazeDisplay[rows * cols - 1] = "E"; // end

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << mazeDisplay[i * cols + j] << " ";
        }
        cout << endl;
    }
}


int main() {
    //creates random maze
    srand(time(0));
    int rows = 40, cols = 40;  // size
    vector<Cell> maze = generateMaze(rows, cols);
    auto graph = graphBuild(maze, rows, cols);
    int start = 0, end = rows * cols - 1;


    cout << "------------------------------------------------------------------" << endl;
    cout << "|  GRAPH PATHFINDING PERFORMANCE                                 |" << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << "|     Choose Pathfinding Algorithm:                             |" << endl;
    cout << "|   1) Dijkstra's Algorithm                                     |" << endl;
    cout << "|   2) A* Algorithm                                             |" << endl;
    cout << "|   3) Compare Both                                             |" << endl;
    cout << "------------------------------------------------------------------" << endl;

    char choice;
    cin >> choice;
    clock_t startTime;
    clock_t endTime;


    if (choice == '1' || choice == '3') {
        startTime = clock();
        vector<int> path = dijkstra(graph, start, end);
        endTime = clock();
        cout << "Dijkstra's Algorithm Results:" << endl;
        cout << "Time Taken: " << fixed << setprecision(5) << double(endTime - startTime) / CLOCKS_PER_SEC << " seconds" << endl;
        dispMaze(maze, path, rows, cols);
    }

    if (choice == '2' || choice == '3') {
        startTime = clock();
        vector<int> path = aStar(graph, maze, start, end);
        endTime = clock();
        cout << "A* Algorithm Results:" << endl;
        cout << "Time Taken: " << fixed << setprecision(5) << double(endTime - startTime) / CLOCKS_PER_SEC << " seconds" << endl;
        dispMaze(maze, path, rows, cols);
    }

    return 0;
}
