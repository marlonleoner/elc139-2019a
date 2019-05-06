//
//  Este programa MPI não contém um erro!
//  
//  Os envios e o recebimentos das mensagens estavam com tags diferentes,
//  com isso, quando P0 enviava uma mensagem, P1 não reconhecia.
//  Deixando o send de um processo e o receive do outro com a mesma tag, o
//  envio das mensagens ocorre normalmente.  
//
//  Uso: mpirun -np 2 <prog>
//  
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
   int numtasks, rank, dest, source, rc;
   int tag_send, tag_receive;
   int inmsg, outmsg = 100;
   MPI_Status stat;

   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   printf("Processo %d iniciando...\n",rank);

   if (rank == 0) {
      dest = source = 1;
      tag_send    = dest;
      tag_receive = rank;
      rc = MPI_Send(&outmsg, 1, MPI_INT, dest, tag_send, MPI_COMM_WORLD);
      printf("P0: Enviei mensagem para processo %d...\n", dest);
      rc = MPI_Recv(&inmsg, 1, MPI_INT, source, tag_receive, MPI_COMM_WORLD, &stat);
      printf("P0: Recebi mensagem do processo %d...\n", source);
   }
   else if (rank == 1) {
      dest = source = 0;
      tag_send    = rank;
      tag_receive = dest;
      rc = MPI_Recv(&inmsg, 1, MPI_INT, source, tag_send, MPI_COMM_WORLD, &stat);
      printf("P1: Recebi mensagem do processo %d...\n", source);
      rc = MPI_Send(&outmsg, 1, MPI_INT, dest, tag_receive, MPI_COMM_WORLD);
      printf("P1: Enviei mensagem para processo %d...\n", dest);
   }

   MPI_Finalize();
}