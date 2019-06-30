#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <sys/time.h>

__global__ void warshall(int nNodes, short int* graph) {

   int blockID = blockIdx.x;
   int tBlocks = gridDim.x;

   int threadID = threadIdx.x;
   int tThreads = blockDim.x;

   for(int k = blockID; k < nNodes; k += tBlocks) {
      for(int i = threadID; i < nNodes; i += tThreads) {
         for (int j = 0; j < nNodes; j++){

            if(graph[i * nNodes + k] + graph[k * nNodes + j] < graph[i * nNodes + j])
               graph[i * nNodes + j] = 1;

         }
      }
   }
}

long wtime() {
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec * 1000000 + t.tv_usec;
}

int read(short int** graph) {

   short int* aux_graph = NULL;
   int nNodes;

   char line[50];
   char* token;
   int size = 50;

   int l;
   int c;

   fgets(line, size, stdin);

   while(!feof(stdin)) {
      token = strtok(line," "); // split using space as divider
      if(*token == 'p') {

         token = strtok(NULL," "); // sp

         token  = strtok(NULL," "); // no. of vertices
         nNodes = atoi(token);

         token = strtok(NULL," "); // no. of directed edges

         // allocate picture array
         cudaMallocManaged(&aux_graph, nNodes * nNodes * sizeof(short int));
         if (aux_graph == NULL) {
            printf("Error in graph allocation: NULL!\n");
            exit(EXIT_FAILURE);
         }

         for(int i = 0; i < nNodes;i++){
            for(int j = 0; j < nNodes;j++){
               aux_graph[i*nNodes+j] = 0;
            }
         }
      }
      else if(*token == 'a') {
         token = strtok(NULL," ");
         l = atoi(token)-1;

         token = strtok(NULL," ");
         c = atoi(token)-1;

         token = strtok(NULL," ");
         aux_graph[l*nNodes+c] = 1;
      }

      fgets(line, size, stdin);
   }

   *(graph) = aux_graph;

   return nNodes;
}

void write(int nNodes, short int* graph) {
   int i, j;

   for(i = 0; i < nNodes; i++) {
      for(j = 0; j < nNodes; j++) {
         printf("%d ", graph[i * nNodes + j]);
      }
      printf("\n");
   }
}


int main(int argc, char *argv[]) {

   int debug = 0;
   if(argc > 1)
      debug = atoi(argv[1]);

   short int* graph  = NULL;
   int        nNodes = read(&graph);

   // start time
   long start_time = wtime();

   // Warshall()
   warshall<<<2, 1024>>>(nNodes, graph);

   // Wait for GPU to finish before accessing on host
   cudaDeviceSynchronize();

   // end time
   long end_time  = wtime();

   printf("warshall(): %ld usec\n", (long)(end_time - start_time));

   if(debug)
      write(nNodes, graph);

   cudaFree(graph);

   return 0;
}