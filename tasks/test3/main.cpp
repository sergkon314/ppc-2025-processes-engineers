#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int rows = 3, cols = 4;
    int full_array[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };
    
    // Создаем тип для строки
    MPI_Datatype ROW_TYPE;
    MPI_Type_vector(cols, 1, 1, MPI_INT, &ROW_TYPE);
    MPI_Type_commit(&ROW_TYPE);
    
    // Процесс 0 рассылает строки всем процессам
    if (rank == 0) {
        for (int dest = 1; dest < size; dest++) {
            if (dest - 1 < rows) {
                MPI_Send(&full_array[dest - 1][0], 1, ROW_TYPE, 
                        dest, 0, MPI_COMM_WORLD);
                printf("Отправлена строка %d процессу %d\n", dest - 1, dest);
            }
        }
    } else if (rank - 1 < rows) {
        int my_row[cols];
        MPI_Recv(my_row, cols, MPI_INT, 0, 0, 
                MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        printf("Процесс %d получил строку: ", rank);
        for (int j = 0; j < cols; j++) {
            printf("%d ", my_row[j]);
        }
        printf("\n");
    }
    
    MPI_Type_free(&ROW_TYPE);
    MPI_Finalize();
    return 0;
}
