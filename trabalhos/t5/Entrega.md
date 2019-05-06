# T5: Primeiros passos com MPI

## Identificação

Nome: Marlon Leoner da Silva Rodrigues<br/>Disciplina: [Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2019a)

## Link dos programas

+ Versão em MPI do t2: [Código](src/mpi_dotprod.c)
+ Programa MPI - Pipeline: [Código](src/mpi_pipeline.c)
+ Solução 1(Parte 3): [Código](src/mpi_correto1.c)
+ Solução 2(Parte 3): [Código](src/mpi_correto2.c)

## Parte 1

+ Implemente uma versão em MPI do programa do trabalho t2. [Código](src/mpi_dotprod.c)
+ Avalie o desempenho do programa conforme as instruções do trabalho t2.

## Parte 2

Implemente um programa MPI que transporte uma mensagem em um pipeline formado por processos de 0 a NP-1. [Código](src/mpi_pipeline.c)

## Parte 3

+ O programa [mpi_errado1.c](src/mpi_errado1.c) não funciona como esperado. O erro, no código, encontrava-se na tag das funções de envio e recebimento das mensagens. Como os processos possuiam tags diferentes, quando um deles enviava uma mensagem, o outro não conseguia receber tal mensagem. A correção feita: definir valores iguais nas tags dos métodos de envio e recebimento das mensagens de ambos os processos.</br>Correção: [mpi_correto1.c](src/mpi_correto1.c).

+ O programa [mpi_errado2.c](src/mpi_errado1.c) também não funciona como esperado. Este código não possuia o comando que finaliza o MPI(MPI_Finalize()), sendo assim, foi apenas necessário adicionar tal comando ao final do código.</br>Correção: [mpi_correto2.c](src/mpi_correto2.c). 

## Referências 
- [Message Passing Interface (MPI)](https://computing.llnl.gov/tutorials/mpi/)