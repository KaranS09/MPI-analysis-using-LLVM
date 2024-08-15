#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the rank of the process in MPI_COMM_WORLD
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // --------------------------------------------
    // Communication in MPI_COMM_WORLD (Default Communicator)
    // --------------------------------------------

    if (rank == 0)
    {
        int data = 100; // Data to send
        // Process 0 sends data to Process 1 with tag 0
        MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else if (rank == 1)
    {
        int data; // Buffer to receive data
        // Process 1 receives data from Process 0 with tag 0
        MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received data: %d\n", data);
    }

    // --------------------------------------------
    // Custom Communicator (custom_comm)
    // --------------------------------------------
    // Split MPI_COMM_WORLD into two groups: even ranks and odd ranks
    // Each group forms a new communicator

    MPI_Comm custom_comm;
    MPI_Comm_split(MPI_COMM_WORLD, rank % 2, rank, &custom_comm);

    if (rank == 2)
    {
        int data = 200; // Data to send
        // Process 2 sends data to Process 3 with tag 1 within custom_comm
        MPI_Send(&data, 1, MPI_INT, 3, 1, custom_comm);
    }
    else if (rank == 3)
    {
        int data; // Buffer to receive data
        // Process 3 receives data from Process 2 with tag 1 within custom_comm
        MPI_Recv(&data, 1, MPI_INT, 2, 1, custom_comm, MPI_STATUS_IGNORE);
        printf("Process 3 received data: %d\n", data);
    }

    // --------------------------------------------
    // Adding More Processes with MPI_COMM_WORLD
    // --------------------------------------------

    if (rank == 4)
    {
        int data = 300; // Data to send
        // Process 4 sends data to Process 5 with tag 2
        MPI_Send(&data, 1, MPI_INT, 5, 2, MPI_COMM_WORLD);
    }
    else if (rank == 5)
    {
        int data; // Buffer to receive data
        // Process 5 receives data from Process 4 with tag 2
        MPI_Recv(&data, 1, MPI_INT, 4, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 5 received data: %d\n", data);
    }

    // --------------------------------------------
    // Additional Custom Communicator (custom_comm2)
    // --------------------------------------------
    // Split MPI_COMM_WORLD into three groups based on rank mod 3
    // Each group forms a new communicator

    MPI_Comm custom_comm2;
    MPI_Comm_split(MPI_COMM_WORLD, rank % 3, rank, &custom_comm2);

    if (rank == 6)
    {
        int data = 400; // Data to send
        // Process 6 sends data to Process 7 with tag 3 within custom_comm2
        MPI_Send(&data, 1, MPI_INT, 7, 3, custom_comm2);
    }
    else if (rank == 7)
    {
        int data; // Buffer to receive data
        // Process 7 receives data from Process 6 with tag 3 within custom_comm2
        MPI_Recv(&data, 1, MPI_INT, 6, 3, custom_comm2, MPI_STATUS_IGNORE);
        printf("Process 7 received data: %d\n", data);
    }

    // Free the communicators before finalizing
    MPI_Comm_free(&custom_comm);
    MPI_Comm_free(&custom_comm2);

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}
