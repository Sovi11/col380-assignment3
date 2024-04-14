#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <mpi.h>

using namespace std;

bool debug = true ;

int main(){
    // This is to learn basics and barriers
    MPI_Init(NULL, NULL);
    int world_size;
    int world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    cout << "Hello from " << world_rank << " of " << world_size << endl;
    MPI_Barrier(MPI_COMM_WORLD);
    cout << "Bye from " << world_rank << " of " << world_size << endl;
    MPI_Finalize() ;

}