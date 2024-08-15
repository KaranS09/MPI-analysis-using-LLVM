#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(NULL, NULL);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Communication in MPI_COMM_WORLD
    if (rank == 0)
    {
        int data = 100;
        MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD); // Tag 0
    }
    else if (rank == 1)
    {
        int data;
        MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Tag 0
        printf("Process 1 received data: %d\n", data);
    }

    // Custom Communicator (custom_comm)
    MPI_Comm custom_comm;
    MPI_Comm_split(MPI_COMM_WORLD, rank % 2, rank, &custom_comm);

    if (rank == 2)
    {
        int data = 200;
        MPI_Send(&data, 1, MPI_INT, 3, 1, custom_comm); // Tag 1, custom communicator
    }
    else if (rank == 3)
    {
        int data;
        MPI_Recv(&data, 1, MPI_INT, 2, 1, custom_comm, MPI_STATUS_IGNORE); // Tag 1, custom communicator
        printf("Process 3 received data: %d\n", data);
    }

    // Adding more processes with MPI_COMM_WORLD
    if (rank == 4)
    {
        int data = 300;
        MPI_Send(&data, 1, MPI_INT, 5, 2, MPI_COMM_WORLD); // Tag 2
    }
    else if (rank == 5)
    {
        int data;
        MPI_Recv(&data, 1, MPI_INT, 4, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Tag 2
        printf("Process 5 received data: %d\n", data);
    }

    // Additional custom communicator (custom_comm2)
    MPI_Comm custom_comm2;
    MPI_Comm_split(MPI_COMM_WORLD, rank % 3, rank, &custom_comm2);

    if (rank == 6)
    {
        int data = 400;
        MPI_Send(&data, 1, MPI_INT, 7, 3, custom_comm2); // Tag 3, custom communicator 2
    }
    else if (rank == 7)
    {
        int data;
        MPI_Recv(&data, 1, MPI_INT, 6, 3, custom_comm2, MPI_STATUS_IGNORE); // Tag 3, custom communicator 2
        printf("Process 7 received data: %d\n", data);
    }

    MPI_Comm_free(&custom_comm);
    MPI_Comm_free(&custom_comm2);
    MPI_Finalize();

    return 0;
}
