#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void analyze_uniform_participation(int rank, int size) {
    if (rank == 0) {
        printf("Analyzing uniform participation of MPI processes...\n");
        printf("Total number of processes: %d\n", size);
        // Add your analysis logic here.
        // For demonstration, we'll just print the rank of each process.
        for (int i = 0; i < size; i++) {
            printf("Process %d is active.\n", i);
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Command-line option parsing
    int analyze_uniform = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-analyze-mpi-uniform-participation") == 0) {
            analyze_uniform = 1;
        }
    }

    // Handle the analysis option
    if (analyze_uniform) {
        analyze_uniform_participation(rank, size);
    }

    // Original MPI communication
    if (rank == 0) {
        int data = 100;
        MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else if (rank == 1) {
        int data;
        MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received data: %d\n", data);
    }

    MPI_Finalize();
    return 0;
}

