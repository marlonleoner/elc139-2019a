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
+ Avalie o desempenho do programa conforme as instruções do trabalho t2. Todos os testes foram realizados com a variável 'repetição' setado em 1000, variando o valor do tamanho do vetor e a quantidade de processos. Nas tabelas abaixo temos os tempos de execução e o speedup das execuções.

	+ Tempo de execução
	
    | Tam. Vetor | 1Processo(s) | 2Processo(s) | 4Processo(s) | 8Processo(s) |
    |:----------:|:------------:|:------------:|:------------:|:------------:|
    | 100000     |   570545     |   313246     |   222777     |   214309     |
    | 500000     |   2975681    |   1489083    |   1172833    |   1141165    |
    | 1000000    |   5966520    |   3014160    |   2361977    |   2283987    |
    | 2000000    |   11928061   |   6069409    |   4685436    |   4689777    |

    + Speedup

    | Tam. Vetor | 2Processo(s) | 4Processo(s) | 8Processo(s) |
    |:----------:|:------------:|:------------:|:------------:|
    | 100000     |   1,82       |   2,56       |   2,66       |
    | 500000     |   1,99       |   2,53       |   2,60       |
    | 1000000    |   1,97       |   2,52       |   2,61       |
    | 2000000    |   1,96       |   2,54       |   2,54       |

    Pode-se perceber que há uma melhora do tempo de execução do programa com 2 processos em relação ao programa sequêncial, houve um ganho na execução com 4 processos em relação ao programa com 2 processos, porém não há um grande ganho quando excedemos o número threads da máquina.

## Parte 2

Implemente um programa MPI que transporte uma mensagem em um pipeline formado por processos de 0 a NP-1. [Código](src/mpi_pipeline.c)

## Parte 3

+ O programa [mpi_errado1.c](src/mpi_errado1.c) não funciona como esperado. O erro, no código, encontrava-se na tag das funções de envio e recebimento das mensagens. Como os processos possuiam tags diferentes, quando um deles enviava uma mensagem, o outro não conseguia receber tal mensagem. A correção feita: definir valores iguais nas tags dos métodos de envio e recebimento das mensagens de ambos os processos.</br>Correção: [mpi_correto1.c](src/mpi_correto1.c).

+ O programa [mpi_errado2.c](src/mpi_errado1.c) também não funciona como esperado. Este código não possuia o comando que finaliza o MPI(MPI_Finalize()), sendo assim, foi apenas necessário adicionar tal comando ao final do código.</br>Correção: [mpi_correto2.c](src/mpi_correto2.c). 

## Referências 
- [Message Passing Interface (MPI)](https://computing.llnl.gov/tutorials/mpi/)