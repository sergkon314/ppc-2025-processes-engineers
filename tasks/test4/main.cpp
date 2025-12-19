#include <mpi.h>
#include <vector>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    const int ROWS = 4;
    const int COLS = 3;
    // Use a 1D vector to represent a 2D matrix (ROWS x COLS) in row-major order
    std::vector<int> matrix(ROWS * COLS); 

    if (world_rank == 0) {
        // Initialize the matrix with some values
        for (int i = 0; i < ROWS * COLS; ++i) {
            matrix[i] = i;
        }
        // Example: matrix looks like
        // 0 1 2
        // 3 4 5
        // 6 7 8
        // 9 10 11
    }

    // --- Create a new MPI Datatype for a single column ---
    MPI_Datatype column_type;
    int count = ROWS;        // Number of blocks (rows)
    int blocklength = 1;     // Number of elements in each block (one per row)
    // Stride is the distance between the start of one block and the next
    int stride = COLS;       // Stride in terms of the number of 'oldtype' elements

    // The oldtype is the basic data type of elements in the vector
    MPI_Type_vector(count, blocklength, stride, MPI_INT, &column_type);
    MPI_Type_commit(&column_type); // Commit the new type before use

    // Example MPI communication (sending the first column from rank 0 to rank 1)
    if (world_rank == 0) {
        // Send the first column (starting at the beginning of the data array)
        MPI_Send(matrix.data(), 1, column_type, 1, 0, MPI_COMM_WORLD); 
    } else {
        std::vector<int> received_column(ROWS); // Vector to store the received column
        // Receive into a contiguous vector
        MPI_Recv(received_column.data(), ROWS, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        std::cout << "Rank 1 received column: ";
        for (int val : received_column) {
            std::cout << val << " "; // Expected: 0 3 6 9
        }
        std::cout << std::endl;
    }

    // Clean up the created datatype
    MPI_Type_free(&column_type);
    MPI_Finalize();
    return 0;
}
