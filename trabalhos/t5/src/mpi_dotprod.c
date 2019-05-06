#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include <mpi.h>

typedef struct {
   double *a;
   double *b;
   double c;
   int wsize;
   int repeat;
} dotdata_t;

dotdata_t dotdata;

/*
 * Funcao executada por uma thread
 */
void dotprod_worker() {
   int i, k;

   double *a = dotdata.a;
   double *b = dotdata.b;

   for (k = 0; k < dotdata.repeat; k++) {
      dotdata.c = 0.0;
      for (i = 0; i < dotdata.wsize; i++) {
         dotdata.c += (a[i] * b[i]);
      }
   }
}

/*
 * Tempo (wallclock) em microssegundos
 */
long wtime() {
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec * 1000000 + t.tv_usec;
}

/*
 * Preenche vetor
 */
void fill(double *a, int size, double value) {
   int i;
   for (i = 0; i < size; i++)
      a[i] = value;
}

/*
 * Funcao principal
 */
int main(int argc, char **argv) {
   int  wsize;          // Tamanho do array
   int repeat;          // Número de repetições
   int tasks;           // Número total de processos
   int process;         // Número do processo
   int source;          // Fonte da mensagem
   double start;        // Tempo inicial
   double end;          // Tempo final
   double total  = 0.0; // Armazena o resultado de todas as threads
   double result;       // Resultado de cada processo

   MPI_Status status;   // "status" de uma operação efetuada

   MPI_Init(&argc, &argv);                  // Inicialização do MPI
   MPI_Comm_rank(MPI_COMM_WORLD, &process); // Número do processo
   MPI_Comm_size(MPI_COMM_WORLD, &tasks);   // Número total de processos

   if (argc != 3) {
      printf("Error! Try: %s <worksize> <repetitions>\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   wsize  = atoi(argv[1]);  // worksize = tamanho do vetor de cada thread
   repeat = atoi(argv[2]);  // numero de repeticoes dos calculos (para aumentar carga)
   if(wsize < 1 || repeat < 1) {
      printf("Error! <worksize> and <repetitions> must be at least 1.\n");
      exit(EXIT_FAILURE);
   }

   // Cria vetores
   dotdata.a = (double *)malloc(wsize * sizeof(double));
   fill(dotdata.a, wsize, 0.01);
   dotdata.b = (double *)malloc(wsize * sizeof(double));
   fill(dotdata.b, wsize, 1.00);

   // Define valores
   dotdata.c      = 0.0;
   dotdata.wsize  = wsize / tasks;
   dotdata.repeat = repeat;

   // Trabalho extra vai para o processo 0
   if(process == 0)
      dotdata.wsize += (wsize % tasks);

   // Calculo do produto escalar
   start = wtime();
   dotprod_worker();
   end   = wtime();

   // Exibe informações do processo
   printf("P%d -> Result: %.2f - Size: %d\n", process, dotdata.c, dotdata.wsize);

   // Processos com id diferente de 0 enviam mensagens
   if (process != 0) {
      // Envia mensagem ao processo 0
      MPI_Send(&dotdata.c, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
   }
   // Processo 0 recebe a mensagem dos outros processos
   else {
      for(source = 1; source < tasks; source++) {
         MPI_Recv(&result, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, &status);
         dotdata.c += result;
      }

      // Mostra resultado e estatisticas da execucao
      printf("%.2lf\n%d thread(s), %.0fusec\n", dotdata.c, tasks, (end-start));
      fflush(stdout);
   }   

   free(dotdata.a);
   free(dotdata.b);

   MPI_Finalize(); // Finalização do MPI

   return EXIT_SUCCESS;
}