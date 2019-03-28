#include <omp.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
  double *a;
  double *b;
  double c;
  int wsize;
  int repeat;
} dotdata_t;

dotdata_t dotdata;

void dotprod_worker(int ID)
{
  int i, k;
  long offset = (long)ID;
  double *a = dotdata.a;
  double *b = dotdata.b;
  int wsize = dotdata.wsize;
  int start = offset * wsize;
  int end = start + wsize;
  double mysum;

  for (k = 0; k < dotdata.repeat; k++)
  {
    mysum = 0.0;
    for (i = start; i < end; i++)
    {
      mysum += (a[i] * b[i]);
    }
  }
#pragma omp critical
  {
    dotdata.c += mysum;
  }

  return;
}

/*
 * Tempo (wallclock) em microssegundos
 */
long wtime()
{
  struct timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec * 1000000 + t.tv_usec;
}

/*
 * Preenche vetor
 */
void fill(double *a, int size, double value)
{
  int i;
  for (i = 0; i < size; i++)
  {
    a[i] = value;
  }
}

int main(int argc, char **argv)
{

  int nthreads, wsize, repeat;
  long start_time, end_time;

  if ((argc < 4))
  {
    printf("Uso: %s <nthreads> <worksize> <repetitions>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  nthreads = atoi(argv[1]);
  wsize = atoi(argv[2]);
  repeat = atoi(argv[3]);

  // Cria vetores
  dotdata.a = (double *)malloc(wsize * nthreads * sizeof(double));
  fill(dotdata.a, wsize * nthreads, 0.01);
  dotdata.b = (double *)malloc(wsize * nthreads * sizeof(double));
  fill(dotdata.b, wsize * nthreads, 1.0);
  dotdata.c = 0.0;
  dotdata.wsize = wsize;
  dotdata.repeat = repeat;

  start_time = wtime();
#pragma omp parallel num_threads(nthreads) shared(dotdata)
  {
    int ID = omp_get_thread_num();
    dotprod_worker(ID);
  }
  end_time = wtime();

  // Mostra resultado e estatisticas da execucao
  // printf("%f\n", dotdata.c);
  // printf("%ld usec\n", (long)(end_time - start_time));
  printf("%ld\n", (long)(end_time - start_time));
  fflush(stdout);

  free(dotdata.a);
  free(dotdata.b);

  return EXIT_SUCCESS;
}
