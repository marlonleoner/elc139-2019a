#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include <mpi.h>

/*
 * Funcao principal
 */
int main(int argc, char **argv) {
   int tasks;         // Número total de processos
   int process;       // Número do processo
   int value;         // Valor a ser repassado

   MPI_Status status; // "status" de uma operação efetuada

   MPI_Init(&argc, &argv);                  // Inicialização do MPI
   MPI_Comm_rank(MPI_COMM_WORLD, &process); // Número do processo
   MPI_Comm_size(MPI_COMM_WORLD, &tasks);   // Número total de processos

   if(tasks < 2) {
      printf("Error! <num_tasks> must be at least 2.\n");
      return EXIT_FAILURE;
   }

   // N = Numero total de processos
   // Processo 0
   if (process == 0) {
      value = 10;
      printf("=== Inicio do PIPELINE ===\n");
      printf("Starting w/ %d\n", value);
      printf("P%d send %d to P%d\n", process, value, process+1);
      MPI_Send(&value, 1, MPI_INT, process+1, 0, MPI_COMM_WORLD);
   }
   // Processo 1 ~ (N-2)
   else if(process > 0 && process < tasks - 1) {
      MPI_Recv(&value, 1, MPI_DOUBLE, process-1, 0, MPI_COMM_WORLD, &status);
      printf("P%d receive %d from P%d\n", process, value, process-1);
      value += 10;
      printf("P%d send %d to P%d\n", process, value, process+1);
      MPI_Send(&value, 1, MPI_INT, process+1, 0, MPI_COMM_WORLD);
   }
   // Processo (N-1)
   else {
      MPI_Recv(&value, 1, MPI_DOUBLE, process-1, 0, MPI_COMM_WORLD, &status);
      printf("P%d receive %d from P%d\n", process, value, process-1);
      printf("Ending w/ %d\n", value);
      printf("=== End of Pipeline ===\n");
   }   

   MPI_Finalize(); // Finalização do MPI

   return EXIT_SUCCESS;
}