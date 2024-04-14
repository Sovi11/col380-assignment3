#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <mpi.h>

#define MAZE_SIZE 64
#define WALL 1
#define PATH 0
#define INF 999999

using namespace std;

bool debug = true ; 

struct Edge {
    int src, dest, weight;
};

// Function to generate a random number between low and high
int getRandom(int low, int high) {
    return rand() % (high - low + 1) + low;
}

// Function to create a perfect maze using Kruskal's algorithm
void createMazeKruskal(int maze[MAZE_SIZE][MAZE_SIZE]) {
    srand(time(NULL));
    vector<Edge> edges;

    // Create all possible edges
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            if (i > 0) edges.push_back({i * MAZE_SIZE + j, (i - 1) * MAZE_SIZE + j, getRandom(1, 100)});
            if (j > 0) edges.push_back({i * MAZE_SIZE + j, i * MAZE_SIZE + (j - 1), getRandom(1, 100)});
        }
    }

    // Shuffle the edges
    random_device rd;
    mt19937 g(rd());
    shuffle(edges.begin(), edges.end(), g);

    // Initialize Union-Find data structure
    int parent[MAZE_SIZE * MAZE_SIZE];
    for (int i = 0; i < MAZE_SIZE * MAZE_SIZE; i++)
        parent[i] = i;

    // Kruskal's algorithm
    for (Edge edge : edges) {
        int srcParent = parent[edge.src];
        int destParent = parent[edge.dest];
        if (srcParent != destParent) {
            maze[edge.src / MAZE_SIZE][edge.src % MAZE_SIZE] = PATH;
            maze[edge.dest / MAZE_SIZE][edge.dest % MAZE_SIZE] = PATH;
            parent[srcParent] = destParent;
        }
    }
}

// Function to solve maze using DFS algorithm
void solveMazeDFS(int maze[MAZE_SIZE][MAZE_SIZE], int x, int y) {
    if (x < 0 || x >= MAZE_SIZE || y < 0 || y >= MAZE_SIZE || maze[x][y] == WALL) return;
    if (x == MAZE_SIZE - 1 && y == 0) {
        maze[x][y] = PATH;
        return;
    }
    maze[x][y] = WALL;

    // Explore in all four directions
    solveMazeDFS(maze, x + 1, y);
    solveMazeDFS(maze, x - 1, y);
    solveMazeDFS(maze, x, y + 1);
    solveMazeDFS(maze, x, y - 1);
}

// Function to solve maze using Dijkstra's algorithm
void solveMazeDijkstra(int maze[MAZE_SIZE][MAZE_SIZE]) {
    // Define MPI variables
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Initialize distances
    int distances[MAZE_SIZE][MAZE_SIZE];
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            distances[i][j] = INF;
        }
    }

    // Initialize source node distance to 0
    if (rank == 0)
        distances[0][MAZE_SIZE - 1] = 0;

    // Synchronize processes
    MPI_Barrier(MPI_COMM_WORLD);

    // Dijkstra's algorithm
    for (int count = 0; count < MAZE_SIZE * MAZE_SIZE; count++) {
        int minDistance = INF;
        int minIndexX, minIndexY;

        // Find the vertex with the minimum distance
        for (int i = 0; i < MAZE_SIZE; i++) {
            for (int j = 0; j < MAZE_SIZE; j++) {
                if (distances[i][j] < minDistance && !maze[i][j]) {
                    minDistance = distances[i][j];
                    minIndexX = i;
                    minIndexY = j;
                }
            }
        }

        // Broadcast minimum distance vertex
        int minIndex[2] = {minIndexX, minIndexY};
        MPI_Bcast(minIndex, 2, MPI_INT, 0, MPI_COMM_WORLD);

        // If no vertex found, break
        if (minDistance == INF)
            break;

        // Update distances
        if (rank == 0) {
            distances[minIndexX][minIndexY] = INF;
        }

        // Update distances in each process
        if (rank == 0) {
            for (int i = 0; i < size; i++) {
                MPI_Send(&minIndex, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }

        // Receive minimum distance vertex
        int recvIndex[2];
        MPI_Recv(recvIndex, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int x = recvIndex[0];
        int y = recvIndex[1];

        // Update distances in each process
        for (int i = 0; i < MAZE_SIZE; i++) {
            for (int j = 0; j < MAZE_SIZE; j++) {
                if (!maze[i][j]) {
                    int distance = abs(i - x) + abs(j - y) + minDistance;
                    if (distance < distances[i][j])
                        distances[i][j] = distance;
                }
            }
        }
    }

    // Synchronize processes
    MPI_Barrier(MPI_COMM_WORLD);

    // Print shortest path distance for process 0
    if (rank == 0) {
        cout << "Shortest path distance using Dijkstra's algorithm: " << distances[MAZE_SIZE - 1][0] << endl;
    }
}

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);
    if (debug) cout << "MPI Initialized" << endl ;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (debug) cout << "Rank: " << rank << endl ;
    // Say hello to rank 1 from rank 0

    // int maze[MAZE_SIZE][MAZE_SIZE];

    // //Create maze using Kruskal's algorithm
    // createMazeKruskal(maze);

    // // Solve maze using DFS algorithm
    // if (rank == 0) {
    //     solveMazeDFS(maze, 0, MAZE_SIZE - 1);
    //     cout << "Maze solved using DFS algorithm." << endl;
    // }

    // // Synchronize processes
    // MPI_Barrier(MPI_COMM_WORLD);

    // // Solve maze using Dijkstra's algorithm
    // solveMazeDijkstra(maze);

    MPI_Finalize();
    return 0;
}