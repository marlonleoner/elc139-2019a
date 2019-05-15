# T6: Comunicação coletiva em MPI

## Identificação

Nome: Marlon Leoner da Silva Rodrigues<br/>Disciplina: [Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2019a)

## Link dos programas

+ Programa Original: [matrix_mult_sr.c](src/matrix_mult_sr.c)
+ Programa Alterado: [matrix_mult_alterado.c](src/matrix_mult_alterado.c)

## Parte 1

+ No código fornecido, são feitas várias chamadas de send/recv que podem ser trocadas por outras funções. Você deve descobrir quais são essas funções, fazer a alteração e testar se o resultado é o mesmo.

    #### Funções

    As funções utilizadas para realizar a substituição das chamadas de send/recv são: MPI_Bcast(), MPI_Scatter() e MPI_Gather(). Tais métodos são utilizados para comunicação e multiplicação.

    + **MPI_Bcast()** possui a responsabilidade de copiar os dados do processo raiz para os outros processos. É possível obter a mesma funcionalidade utilizando várias chamadas de envio(sends), como foi utilizado no código original.
        ```c
        MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
        ```

    + **MPI_Scatter()** é onde ocorre o envio de diferentes dados, partindo do processo raiz, para cada processo. O resultado é o mesmo que o processo raiz realizar *nprocs* vezes a chamada de envio(send) e cada processo realizar a chamada de recebimento(recv).
        ```c
        MPI_Scatter(void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm)
        ```

    + **MPI_Gather()** deve enviar o conteúdo de seu *buffer* de envio para o processo raiz. O processo raiz recebe as mensagens e as armazena. Cada processo realizando a chamada de envio(send) e o processo raiz realizando chamadas de recebimento(recv) algumas vezes(*nprocs*), é possível obter o mesmo comportamento.
    **Obs.**: o *MPI_Gather()* requer que todos os processos, incluindo a raiz, enviem a mesma quantidade de dados e que os dados sejam do mesmo tipo.
        ```c
        MPI_Gather(void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm)
        ```

    #### Alterações

    Após entender o que cada função realiza e conhecer quais operações tais funções podem substituir, temos o [código](src/matrix_mult_alterado.c) alterado, obtido apartir de alterações realizadas sobre o [código](src/matrix_mult_sr.c) original. </br>

    - O bloco *if/else* foi substituido pelas chamadas das funções *MPI_Bcast()* e *MPI_Scatter()*. Nos trechos de código abaixo é possível visualizar as alterações realizadas.
        + Original.
        ```c
        if (myrank == 0) {
            for (int i = 1; i < nproc; ++i){
                int lFrom = i * SIZE/nproc;
                int lTo = (i+1) * SIZE/nproc;
                MPI_Send(B, SIZE*SIZE, MPI_INT, i, tag_A, MPI_COMM_WORLD);
                MPI_Send(A[lFrom], (lTo - lFrom) * SIZE, MPI_INT, i, tag_B, MPI_COMM_WORLD);
            }
        } else {
            MPI_Recv(B, SIZE*SIZE, MPI_INT, MPI_ANY_SOURCE, tag_A, MPI_COMM_WORLD, &status);
            MPI_Recv(A[from], (to - from)*SIZE, MPI_INT, MPI_ANY_SOURCE, tag_B, MPI_COMM_WORLD, &status);            
        }
        ```
        + Alterado
        ```c
        // Broadcast
        MPI_Bcast(B, (SIZE * SIZE), MPI_INT, 0, MPI_COMM_WORLD);
        // Divisão da matriz A com todos os processos
        MPI_Scatter(A[from], (SIZE * SIZE) / nproc, MPI_INT, A[from], (SIZE * SIZE) / nproc, MPI_INT, 0, MPI_COMM_WORLD);
        ```

## Referências 
- [Message Passing Interface (MPI)](https://computing.llnl.gov/tutorials/mpi/)
- [Web pages for MPI Routines](https://www.mpich.org/static/docs/v3.1/www3/)
- [MPI Programming — Part 2](https://www.cs.usask.ca/~spiteri/CMPT851/notes/MPIprogramming2.pdf)
