#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include <mpi.h>

#define SIZE 960      /* Size of matrices */

int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];

/**
 * Adaptado de www.mpitutorial.com
 * Exemplo de implementação do MPI_Bcast usando MPI_Send e MPI_Recv
 */
void sr_bcast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator) {

   int myrank;     // "rank" do processo
   int p;          // numero de processos

   MPI_Comm_rank(communicator, &myrank);
   MPI_Comm_size(communicator, &p);

   if (myrank == root) {
      // Se for o rank root, enviar o dado para cada um dos demais
      int i;
      for (i = 0; i < p; i++) {
         if (i != myrank) {
            MPI_Send(data, count, datatype, i, 0, communicator);
         }
      }
   } else {
      // Se não for o rank root, recebe o dade envidado pelo rank root
      MPI_Recv(data, count, datatype, root, 0, communicator, MPI_STATUS_IGNORE);
   }
}


/**
 * Preenche uma matriz
 */
void fill_matrix(int m[SIZE][SIZE]) {
   static int n=0;
   int i, j;
   for (i=0; i<SIZE; i++)
      for (j=0; j<SIZE; j++)
         m[i][j] = n++;
}

/**
 * Exibe os dados de uma matriz
 */
void print_matrix(int m[SIZE][SIZE]) {
   int i, j = 0;
   for (i=0; i<SIZE; i++) {
      printf("\n\t| ");
      for (j=0; j<SIZE; j++)
         printf("%*d", 6, m[i][j]);
   printf("|");
   }
}

/**
 * Tempo (wallclock) em microssegundos
 */
long wtime() {
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec * 1000000 + t.tv_usec;
}

int main(int argc, char *argv[]) {
   int myrank, nproc, from, to, i, j, k;
   int root = 0;
   long start;    // Tempo final
   long end;      // Tempo inicial

   MPI_Status status;

   MPI_Init (&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &myrank); /* who am i */
   MPI_Comm_size(MPI_COMM_WORLD, &nproc);  /* number of processors */

   if (SIZE%nproc != 0) {
      if (myrank==0) printf("Matrix size not divisible by number of processors\n");
      MPI_Finalize();
      return -1;
   }

   from = (myrank)   * SIZE/nproc;
   to   = (myrank+1) * SIZE/nproc;

   /* Process 0 fills the input matrices and broadcasts them to the rest */
   /* (actually, only the relevant stripe of A is sent to each process) */
   if (myrank == 0) {
      fill_matrix(A);
      fill_matrix(B);

      start = wtime();
   }

   // Broadcast
   // MPI_Bcast(B, (SIZE * SIZE), MPI_INT, 0, MPI_COMM_WORLD);
   if (myrank == root) {
      sr_bcast(B, (SIZE * SIZE), MPI_INT, root, MPI_COMM_WORLD);
   } else {
      sr_bcast(B, (SIZE * SIZE), MPI_INT, root, MPI_COMM_WORLD);
   }

   // Divisão da matriz A com todos os processos
   MPI_Scatter(A[from], (SIZE * SIZE) / nproc, MPI_INT, A[from], (SIZE * SIZE) / nproc, MPI_INT, 0, MPI_COMM_WORLD);

   printf("computing slice %d (from row %d to %d)\n", myrank, from, to-1);
   for (i=from; i<to; i++) {
      for (j=0; j<SIZE; j++) {
         C[i][j]=0;
         for (k=0; k<SIZE; k++){
            C[i][j] += A[i][k]*B[k][j];
         }
      }
   }

   MPI_Gather (C[from], (SIZE*SIZE)/nproc, MPI_INT, C, (SIZE*SIZE)/nproc, MPI_INT, 0, MPI_COMM_WORLD);

   MPI_Barrier(MPI_COMM_WORLD);

   if (myrank==0) {
      end = wtime();
      printf("finished! %dusec\n", (int)(end - start));
   }

   MPI_Finalize();
   return 0;
}

