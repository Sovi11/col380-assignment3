// idea : the maze is a 64 by 64 square with edges between adjacent cells which represents not the walls but the paths

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <queue>
#include <climits>

using namespace std;
#define SIZEMAZE 5

struct maze_point
{
    int x ; 
    int y ;
    bool upedge = false ; 
    bool downedge = false ;
    bool leftedge = false ;
    bool rightedge = false ;

};

int find(int parent[], int i){
    if (parent[i] != i){
        return find(parent, parent[i]);
    }
    return i ;
}

void Union(int parent[], int x, int y){
    int xset = find(parent, x);
    int yset = find(parent, y);
    if (xset < yset){ 
        parent[xset] = yset;}
    else{
        parent[yset] = xset;
    }
}

maze_point** kruskal(){
    maze_point** maze = new maze_point*[SIZEMAZE];
    for (int i = 0; i < SIZEMAZE; i++){
        maze[i] = new maze_point[SIZEMAZE];
    }
    for (int i = 0; i < SIZEMAZE; i++){
        for (int j = 0; j < SIZEMAZE; j++){
            maze[i][j].x = i;
            maze[i][j].y = j;
        }
    }
    vector<pair<maze_point, maze_point>> edges;
    for (int i = 0; i < SIZEMAZE; i++){
        for (int j = 0; j < SIZEMAZE; j++){
            if (i < SIZEMAZE-1){
                edges.push_back(make_pair(maze[i][j],maze[i+1][j]));
            }
            if (j < SIZEMAZE-1){
                edges.push_back(make_pair(maze[i][j],maze[i][j+1]));
            }
        }
    }
    shuffle(edges.begin(), edges.end(), default_random_engine(time(0)));
    int parent[SIZEMAZE*SIZEMAZE];
    for (int i = 0; i < SIZEMAZE*SIZEMAZE; i++){
        parent[i] = i;
    }
    for (int i = 0; i < edges.size(); i++){
        int x = edges[i].first.x*SIZEMAZE + edges[i].first.y;
        int y = edges[i].second.x*SIZEMAZE + edges[i].second.y;
        if (find(parent, x) != find(parent, y)){
            Union(parent, x, y);
            if (edges[i].first.x == edges[i].second.x){
                if (edges[i].first.y < edges[i].second.y){
                    maze[edges[i].first.x][edges[i].first.y].rightedge = true;
                    maze[edges[i].second.x][edges[i].second.y].leftedge = true;
                }
                else{
                    maze[edges[i].first.x][edges[i].first.y].leftedge = true;
                    maze[edges[i].second.x][edges[i].second.y].rightedge = true;
                }
            }
            else{
                if (edges[i].first.x < edges[i].second.x){
                    maze[edges[i].first.x][edges[i].first.y].downedge = true;
                    maze[edges[i].second.x][edges[i].second.y].upedge = true;
                }
                else{
                    maze[edges[i].first.x][edges[i].first.y].upedge = true;
                    maze[edges[i].second.x][edges[i].second.y].downedge = true;
                }
            }
        }
    }
    return maze;

}

void print_maze(maze_point** maze){
    for (int i = 0; i < SIZEMAZE*2; i++){
        cout<<endl;
        if (i%2 == 0){
            for (int j = 0; j < SIZEMAZE; j++){
                cout << "+";
                if (maze[i/2][j].upedge){
                    cout << " ";
                }
                else{
                    cout << "-";
                }
            }
            cout << "+";
        }
        else{
            for (int j = 0; j < SIZEMAZE; j++){
                if (maze[i/2][j].leftedge){
                    cout << " ";
                }
                else{
                    cout << "|";
                }
                cout << " ";
            }
            if (maze[i/2][SIZEMAZE-1].rightedge){
                cout << " ";
            }
            else{
                cout << "|";
            }
        }

    }
    cout << endl;
    for (int j = 0; j < SIZEMAZE; j++){
        cout << "+";
        if (maze[SIZEMAZE-1][j].downedge){
            cout << " ";
        }
        else{
            cout << "-";
        }
    }
    cout << "+";
    cout << endl;
    cout<<endl;
    
    
}


void dfs_maze(maze_point** maze, int n, int m, int x, int y, int** visited, vector<pair<int, int>> path){
    if (x < 0 || x >= n || y < 0 || y >= m || visited[x][y] == 1){
        return;
    }
    visited[x][y] = 1;
    path.push_back(make_pair(x, y));
    if (x == n-1 && y == m-1){
        for (int i = 0; i < path.size(); i++){
            cout << path[i].first << " " << path[i].second << endl;
        }
        cout << endl;
    }
    if (maze[x][y].upedge){
        dfs_maze(maze, n, m, x-1, y, visited, path);
    }
    if (maze[x][y].downedge){
        dfs_maze(maze, n, m, x+1, y, visited, path);
    }
    if (maze[x][y].leftedge){
        dfs_maze(maze, n, m, x, y-1, visited, path);
    }
    if (maze[x][y].rightedge){
        dfs_maze(maze, n, m, x, y+1, visited, path);
    }
    visited[x][y] = 0;
    path.pop_back();
}


vector<pair<int,int>> dfs_maze_init(maze_point**maze){
    int** visited = new int*[SIZEMAZE];
    for (int i = 0; i < SIZEMAZE; i++){
        visited[i] = new int[SIZEMAZE];
        for (int j = 0; j < SIZEMAZE; j++){
            visited[i][j] = 0;
        }
    }
    vector<pair<int, int>> path;
    dfs_maze(maze, SIZEMAZE, SIZEMAZE, 0, 0, visited, path);
    for (int i = 0; i < SIZEMAZE; i++){
        delete[] visited[i];
    }
    delete[] visited;
    return path;

}

void print_array(int** maze , int n, int m){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            cout << maze[i][j] << " " ; 
        }
        cout << endl;
    }
}

void print_vector_pair(vector<pair<int,int>> p){
    for (int i = 0; i < p.size(); i++){
        cout << p[i].first << " " << p[i].second << endl;
    }
    cout << endl;
}   

vector<pair<int,int>> bfs_solve(maze_point** maze){
    int n = SIZEMAZE;
    int m = SIZEMAZE;
    int** visited = new int*[n];
    for (int i = 0; i < n; i++){
        visited[i] = new int[m];
        for (int j = 0; j < m; j++){
            visited[i][j] = 0;
        }
    }
    queue<pair<int,int>> q;
    q.push(make_pair(0,0));
    visited[0][0] = 1;
    int parent[n][m];
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            parent[i][j] = -1;
        }
    }
    while (!q.empty()){
        pair<int,int> p = q.front();
        q.pop();
        int x = p.first;
        int y = p.second;
        if (x == n-1 && y == m-1){
            break;
        }
        if (maze[x][y].upedge && x-1 >= 0 && visited[x-1][y] == 0){
            q.push(make_pair(x-1,y));
            visited[x-1][y] = 1;
            parent[x-1][y] = x*m + y;
        }
        if (maze[x][y].downedge && x+1 < n && visited[x+1][y] == 0){
            q.push(make_pair(x+1,y));
            visited[x+1][y] = 1;
            parent[x+1][y] = x*m + y;
        }
        if (maze[x][y].leftedge && y-1 >= 0 && visited[x][y-1] == 0){
            q.push(make_pair(x,y-1));
            visited[x][y-1] = 1;
            parent[x][y-1] = x*m + y;
        }
        if (maze[x][y].rightedge && y+1 < m && visited[x][y+1] == 0){
            q.push(make_pair(x,y+1));
            visited[x][y+1] = 1;
            parent[x][y+1] = x*m + y;
        }
    }
    vector<pair<int,int>> path;
    int x = n-1;
    int y = m-1;
    while (x != -1 && y != -1){
        path.push_back(make_pair(x,y));
        int temp = parent[x][y];
        x = temp/m;
        y = temp%m;
    }
    reverse(path.begin(), path.end());
    for (int i = 0; i < n; i++){
        delete[] visited[i];
    }
    delete[] visited;
    return path;
}

vector<pair<int,int>> djikstra_solve(maze_point** maze){
    int n = SIZEMAZE;
    int m = SIZEMAZE;
    int distance[n][m];
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            distance[i][j] = INT_MAX;
        }
    }
    distance[0][0] = 0;
    priority_queue<pair<int,pair<int,int>>, vector<pair<int,pair<int,int>>>, greater<pair<int,pair<int,int>>>> pq;
    pq.push(make_pair(0, make_pair(0, 0)));
    pair<int,int> prev[n][m];
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            prev[i][j] = make_pair(-1, -1);
        }
    }
    while (!pq.empty()){
        int dist = pq.top().first;
        int row = pq.top().second.first;
        int col = pq.top().second.second;
        pq.pop();
        if (row == n - 1 && col == m - 1){
            vector<pair<int, int>> path;
            while (row != -1 && col != -1){
                path.push_back(make_pair(row, col));
                int newRow = prev[row][col].first;
                int newCol = prev[row][col].second;
                row = newRow;
                col = newCol;
            }
            reverse(path.begin(), path.end());
            return path;
        }
        if (maze[row][col].upedge){
            int newRow = row - 1;
            int newCol = col;
            if (newRow >= 0 && distance[newRow][newCol] > dist + 1){
                distance[newRow][newCol] = dist + 1;
                prev[newRow][newCol] = make_pair(row, col);
                pq.push(make_pair(dist + 1, make_pair(newRow, newCol)));

            }
        }
        if (maze[row][col].downedge){
            int newRow = row + 1;
            int newCol = col;
            if (newRow < n && distance[newRow][newCol] > dist + 1){
                distance[newRow][newCol] = dist + 1;
                prev[newRow][newCol] = make_pair(row, col);
                pq.push(make_pair(dist + 1, make_pair(newRow, newCol)));
            }
        }
        if (maze[row][col].leftedge){
            int newRow = row;
            int newCol = col - 1;
            if (newCol >= 0 && distance[newRow][newCol] > dist + 1){
                distance[newRow][newCol] = dist + 1;
                prev[newRow][newCol] = make_pair(row, col);
                pq.push(make_pair(dist + 1, make_pair(newRow, newCol)));
            }
        }
        if (maze[row][col].rightedge){
            int newRow = row;
            int newCol = col + 1;
            if (newCol < m && distance[newRow][newCol] > dist + 1){
                distance[newRow][newCol] = dist + 1;
                prev[newRow][newCol] = make_pair(row, col);
                pq.push(make_pair(dist + 1, make_pair(newRow, newCol)));
            }
        }

    }
    return vector<pair<int,int>> ();

}



int main(){
    maze_point** maze = kruskal();
    print_maze(maze);
    vector<pair<int,int>> p =  dfs_maze_init(maze);
    print_vector_pair(p);
    vector<pair<int,int>> path = bfs_solve(maze);
    print_vector_pair(path);
    vector<pair<int,int>> path2 = djikstra_solve(maze);
    print_vector_pair(path2);

    return 0;
}