#include <iostream>
#include <vector>
#include <queue>
#include <algorithm> // for shuffle
#include <random>    // for random number generator
#include <chrono>    // for seeding random number generator

using namespace std;

// Define directions: up, down, left, right
int moves[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

// Function to generate a maze with only one path using BFS
vector<vector<int>> generateMaze(int n, int m) {
    vector<vector<int>> maze(n, vector<int>(m, 1)); // Initialize maze with all walls

    // Random starting point
    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    mt19937 rng(seed);
    uniform_int_distribution<int> dist(0, 3);
    int startX = 0 ;
    int startY = 0 ;

    // Mark starting point as visited
    maze[startX][startY] = 0;

    queue<pair<int, int>> q;
    q.push(make_pair(startX, startY));

    // Perform BFS to generate the maze
    while (!q.empty()) {
        pair<int, int> current = q.front();
        q.pop();

        // Randomly shuffle the directions
        vector<int> directions = {0, 1, 2, 3};
        shuffle(directions.begin(), directions.end(), rng);

        // Explore neighbors
        for (int i = 0; i < 4; ++i) {
            int dir = directions[i];
            int newX = current.first + moves[dir][0] * 2; // Move 2 steps
            int newY = current.second + moves[dir][1] * 2; // Move 2 steps

            if (newX >= 0 && newX < n && newY >= 0 && newY < m) {
                // Check if the neighbor cell is within the maze bounds
                if (maze[newX][newY] == 1) {
                    // If the neighbor cell hasn't been visited yet, remove the wall between the current cell and the neighbor
                    maze[current.first + moves[dir][0]][current.second + moves[dir][1]] = 0;
                    maze[newX][newY] = 0;
                    q.push(make_pair(newX, newY));
                }
            }
        }
    }

    return maze;
}

// Function to print the maze
void printMaze(const vector<vector<int>>& maze) {
    for (const auto& row : maze) {
        for (int cell : row) {
            if (cell == 1) {
                cout << "█"; // Wall
            } else {
                cout << " "; // Empty space
            }
        }
        cout << endl;
    }
}

int main() {
    int n = 4; // Number of rows
    int m = 4; // Number of columns

    vector<vector<int>> maze = generateMaze(n, m);

    cout << "Generated Maze:" << endl;
    printMaze(maze);

    return 0;
}
