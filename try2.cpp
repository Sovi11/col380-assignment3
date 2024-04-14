#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <mpi.h>

using namespace std;

bool debug = true ;
void print_array(int arr[], int n){
    for (int i = 0; i < n; i++){
        cout << arr[i] << " " ; 
    }
    cout << endl;
}

int main(){
    MPI_Init(NULL, NULL);
    int world_size;
    int world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    cout << "Hello from " << world_rank << " of " << world_size << endl;
    int arr[10]; 
    if (world_rank ==0)
    {for (int i = 0; i < 10; i++){
        arr[i] = i ; 
    }
    }
    if (world_rank==0) MPI_Send(arr, 10, MPI_INT, 1, 0, MPI_COMM_WORLD);
    if (world_rank==1) MPI_Recv(arr, 10, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    print_array(arr, 10);
    cout << "Bye from " << world_rank << " of " << world_size << endl;

    MPI_Finalize() ;

}