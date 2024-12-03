#include <limits>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <queue>
#include <cmath>
#include <ctime>
#include <iomanip>
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


unordered_map<int, vector<pair<int, double>>> buildGraph(const vector<Cell> &maze, int rows, int cols) {
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


// Display maze w/ path
void dispMaze(const vector<Cell> &maze, const vector<int> &path, int rows, int cols) {
    vector<string> mazeDisplay(rows * cols, ".");
    for (int id : path) {
        mazeDisplay[id] = "*";
    }
    mazeDisplay[0] = "S"; // Start
    mazeDisplay[rows * cols - 1] = "E"; // End
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << mazeDisplay[i * cols + j] << " ";
        }
        cout << endl;
    }
}



int main() {
    srand(time(0));
    int rows = 20, cols = 20;
    vector<Cell> maze = generateMaze(rows, cols);
    auto graph = buildGraph(maze, rows, cols);
    int start = 0, end = rows * cols - 1;

    cout << "------------------------------------------------------------------" << endl;
    cout << "|  GRAPH PATHFINDING PERFORMANCE                                 |" << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << "|  Choose Pathfinding Algorithm:                             |" << endl;
    cout << "|   1) Dijkstra's Algorithm                                     |" << endl;
    cout << "|   2) A* Algorithm                                             |" << endl;
    cout << "|   3) Compare Both                                             |" << endl;
    cout << "------------------------------------------------------------------" << endl;

    int option;
    cin >> option;
    clock_t startTime;
    clock_t endTime;

    if (option == 1) {
        startTime = clock();
        vector<int> path = dijkstra(graph, start, end);
        endTime = clock();
        cout << "Dijkstra's Algorithm Results:" << endl;
        cout << "Time Taken: " << fixed << setprecision(5) << double(endTime - startTime) / CLOCKS_PER_SEC << " seconds" << endl;
        dispMaze(maze, path, rows, cols);
    }

}
