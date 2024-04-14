#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <queue>
#include <climits>
#include <vector>
#include <fstream>

#include <mpi.h>

using namespace std;

enum {
    WALL = 1,
    PATH = 0,
};

bool debug = false ;
bool mpi = false ;
void print_array(int arr[], int n){
    for (int i = 0; i < n; i++){
        cout << arr[i] << " " ; 
    }
    cout << endl;
}

void print_vec(vector<pair<int, int>> v){
    for (int i = 0; i < v.size(); i++){
        cout << v[i].first << "," << v[i].second << endl;
    }
}
int moves[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

bool isValidMove(int** maze, int n, int m , int row, int col, vector<vector<bool>>& visited) {
    return (row >= 0 && row < n && col >= 0 && col < m && maze[row][col] == 1 && !visited[row][col]);
}

bool dfs(int** maze, int n, int m, int row, int col, vector<vector<bool>>& visited, vector<pair<int, int>>& path) {
    // Mark the current cell as visited
    visited[row][col] = true;
    path.push_back(make_pair(row, col));

    // If we reached the destination cell, return true
    if (row == n - 1 && col == m - 1){
        return true;
    }

    // Try all possible moves from the current cell
    for (int i = 0; i < 4; ++i) {
        int newRow = row + moves[i][0];
        int newCol = col + moves[i][1];
        if (isValidMove(maze,n,m ,newRow, newCol, visited)) {
            if (dfs(maze,n,m ,newRow, newCol, visited, path)) {
                return true;
            }
        }
    }

    // If no move leads to the destination, backtrack
    path.pop_back();
    return false;
}


vector<pair<int, int>> findPath(int** maze,int n, int m ) {
    vector<vector<bool>> visited(n, vector<bool>(m, false));

    // Start DFS from the top-left cell
    int startRow = 0, startCol = 0;
    vector<pair<int, int>> path;
    dfs(maze,n,m ,startRow, startCol, visited, path);
    return path;
}
int** read_input(string filename, int& n, int& m) {
    ifstream file(filename);
    string line;
    int temp1 , temp2;
    file >> temp1 >> temp2;
    n = temp1;
    m = temp2;
    if (debug) cout<< n << " " << m << endl;
    if (debug) cout<<temp1 << " " << temp2 << endl;
    int** maze = new int*[n];
    for (int i = 0; i < n; ++i) {
        maze[i] = new int[m];
        for (int j = 0; j < m; ++j) {
            file >> maze[i][j];
        }
    }
    file.close();
    return maze;
}

pair<int, vector<pair<int, int>>> dijkstra(int** maze, int n, int m) {
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    vector<vector<int>> distance(n, vector<int>(m, INT_MAX));
    vector<vector<pair<int, int>>> prev(n, vector<pair<int, int>>(m, make_pair(-1, -1)));

    // Dijkstra's algorithm using priority queue
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> pq;

    // Start from the top-left cell
    pq.push(make_pair(0, make_pair(0, 0)));
    distance[0][0] = 0;

    while (!pq.empty()) {
        int dist = pq.top().first;
        int row = pq.top().second.first;
        int col = pq.top().second.second;
        pq.pop();

        if (row == n - 1 && col == m - 1) {
            // Reconstruct path
            vector<pair<int, int>> path;
            while (row != -1 && col != -1) {
                path.push_back(make_pair(row, col));
                int newRow = prev[row][col].first;
                int newCol = prev[row][col].second;
                row = newRow;
                col = newCol;
            }
            reverse(path.begin(), path.end());
            return make_pair(dist, path);
        }

        // Mark current cell as visited
        visited[row][col] = true;

        // Explore neighbors
        for (int i = 0; i < 4; ++i) {
            int newRow = row + moves[i][0];
            int newCol = col + moves[i][1];
            if (isValidMove(maze, n, m, newRow, newCol, visited)) {
                int newDist = dist + 1; // Assuming each step has unit cost
                if (newDist < distance[newRow][newCol]) {
                    distance[newRow][newCol] = newDist;
                    prev[newRow][newCol] = make_pair(row, col);
                    pq.push(make_pair(newDist, make_pair(newRow, newCol)));
                }
            }
        }
    }

    // Destination not reachable
    return make_pair(-1, vector<pair<int, int>>());
}

// write down a kruskal function that takes in a maze matrix and returns the mst as a matrix

int find(int* parent, int i) {
    if (parent[i] != i)
        parent[i] = find(parent, parent[i]);
    return parent[i];
}

void Union(int* subsets, int x, int y) {
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);
    if (xroot != yroot)
        subsets[xroot] = yroot;
}

int** kruskal(int** maze, int n, int m){
    // create a graph with all the edges
    // sort the edges
    // iterate through the edges and add the edge to the mst if it does not create a cycle
    // return the mst
    int** new_matrix = new int*[n];
    for (int i = 0; i < n; i++){
        new_matrix[i] = new int[m];
        for (int j = 0; j < m; j++){
            new_matrix[i][j] = 0;
        }
    }
    // edges are all 1s
    vector<pair<pair<int, int>,pair<int,int>>> edges;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            if (i < n-1){
                if (maze[i][j] == 1 && maze[i+1][j] == 1){
                    edges.push_back(make_pair(make_pair(i,j),make_pair(i+1,j)));
                }
            }
            if (j < m-1){
                if (maze[i][j] == 1 && maze[i][j+1] == 1){
                    edges.push_back(make_pair(make_pair(i,j),make_pair(i,j+1)));
                }
            }
        }
    }

    shuffle(edges.begin(), edges.end(), default_random_engine(time(0)));
    // dsu type of implementation to check if adding an edge will create a cycle
    int parent[n*m];
    for (int i = 0; i < n*m; i++){
        parent[i] = i;
    }

    for (int i = 0; i < edges.size(); i++){
        int x = edges[i].first.first*m + edges[i].first.second;
        int y = edges[i].second.first*m + edges[i].second.second;
        Union(parent, x, y);
        new_matrix[edges[i].first.first][edges[i].first.second] = 1;
        new_matrix[edges[i].second.first][edges[i].second.second] = 1;
    }
    return new_matrix;


}

void print_array(int** maze , int n, int m){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            cout << maze[i][j] << " " ; 
        }
        cout << endl;
    }
}



int main(){
    if (mpi) MPI_Init(NULL, NULL);
    int world_size;
    int my_rank =0;
    if (mpi)MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    if (mpi)MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    if (my_rank == 0) {
            int n, m;
        int** maze = read_input("maze.txt", n, m);
        vector<pair<int, int>> path = findPath(maze,n,m );
        if (debug) print_array(maze,n,m);
        if (path.size() == 0) {
            cout << "No path found!" << endl;
        } else {
            cout << "Path found!" << endl;
            for (pair<int, int> p : path) {
                cout << p.first << " " << p.second << endl;
            }
        }

        // Use Dijkstra's algorithm to find the shortest path
        pair<int, vector<pair<int, int>>> result = dijkstra(maze, n, m);
        if (result.first == -1) {
            cout << "Destination not reachable!" << endl;
        } else {
            cout << "Shortest path distance using Dijkstra's algorithm: " << result.first << endl;
            for (pair<int, int> p : result.second) {
                cout << p.first << " " << p.second << endl;
            }
        }
        // Use the maze...
        // Don't forget to deallocate memory for maze when done
        for (int i = 0; i < n; ++i) {
            delete[] maze[i];
        }
        delete[] maze;
    }

    if (mpi) MPI_Finalize() ;
}