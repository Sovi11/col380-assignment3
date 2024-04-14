#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;
struct Edge {
    int src, dest, weight;
};

struct Subset {
    int parent, rank;
};

class Maze {
private:
    int rows, cols;
    vector<Edge> edges;

    int find(Subset subsets[], int i) {
        if (subsets[i].parent != i)
            subsets[i].parent = find(subsets, subsets[i].parent);
        return subsets[i].parent;
    }

    void Union(Subset subsets[], int x, int y) {
        int xroot = find(subsets, x);
        int yroot = find(subsets, y);

        if (subsets[xroot].rank < subsets[yroot].rank)
            subsets[xroot].parent = yroot;
        else if (subsets[xroot].rank > subsets[yroot].rank)
            subsets[yroot].parent = xroot;
        else {
            subsets[yroot].parent = xroot;
            subsets[xroot].rank++;
        }
    }

public:
    Maze(int n, int m) : rows(n), cols(m) {}

    void addEdge(int src, int dest) {
        edges.push_back({src, dest});
    }

    void findMST() {
        vector<Edge> result;


        // Allocate memory for subsets
        Subset* subsets = new Subset[rows * cols];

        // Initialize subsets
        for (int i = 0; i < rows * cols; ++i) {
            subsets[i].parent = i;
            subsets[i].rank = 0;
        }

        int edgeCount = 0;
        for (const auto& edge : edges) {
            int srcParent = find(subsets, edge.src);
            int destParent = find(subsets, edge.dest);

            if (srcParent != destParent) {
                result.push_back(edge);
                Union(subsets, srcParent, destParent);
                edgeCount++;
            }

            if (edgeCount == rows * cols - 1) // MST has (V-1) edges
                break;
        }

        // Output the MST as a matrix
        vector<vector<int>> mst(rows, vector<int>(cols, 0));
        for (const auto& edge : result) {
            int srcRow = edge.src / cols;
            int srcCol = edge.src % cols;
            int destRow = edge.dest / cols;
            int destCol = edge.dest % cols;

            // Mark the cells in the MST path
            mst[srcRow][srcCol] = 1;
            mst[destRow][destCol] = 1;
        }

        // Print the MST matrix
        for (const auto& row : mst) {
            for (int cell : row) {
                cout << cell << " ";
            }
            cout << endl;
        }

        delete[] subsets;
    }
};

int main() {
    // Example binary maze represented as a matrix
    vector<vector<int>> maze = {
        {1, 0, 1, 0},
        {1, 1, 1, 0},
        {0, 1, 0, 1},
        {0, 1, 1, 1}
    };

    int rows = maze.size();
    int cols = maze[0].size();

    Maze mazeGraph(rows, cols);

    // Add horizontal edges
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols - 1; ++j) {
            int weight = (maze[i][j] == 1 && maze[i][j + 1] == 1) ? 0 : 1;
            mazeGraph.addEdge(i * cols + j, i * cols + j + 1, weight);
        }
    }

    // Add vertical edges
    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < rows - 1; ++i) {
            int weight = (maze[i][j] == 1 && maze[i + 1][j] == 1) ? 0 : 1;
            mazeGraph.addEdge(i * cols + j, (i + 1) * cols + j, weight);
        }
    }

    cout << "Minimum Spanning Tree (MST) of the maze:" << endl;
    mazeGraph.findMST();

    return 0;
}