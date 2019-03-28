# T2: Programação Paralela Multithread

## Identificação

Nome: Marlon Leoner da Silva Rodrigues
Disciplina: [Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2019a)

## Pthreads

1.  Explique como se encontram implementadas as 4 etapas de projeto.

- Particionamento: Dividir o problema. No código esta etapa encontra-se representada na função dotprod_worker(), responsável por "quebrar" o problema em tarefas menores.

  ```c
  void *dotprod_worker(void *arg)
  {
   int i, k;
   long offset = (long) arg;
   double *a = dotdata.a;
   double *b = dotdata.b;
   int wsize = dotdata.wsize;
   int start = offset*wsize;
   int end = start + wsize;
   double mysum;

   for (k = 0; k < dotdata.repeat; k++) {
      mysum = 0.0;
      for (i = start; i < end ; i++)  {
         mysum += (a[i] * b[i]);
      }
   }

   pthread_mutex_lock (&mutexsum);
   dotdata.c += mysum;
   pthread_mutex_unlock (&mutexsum);

   pthread_exit((void*) 0);
  }
  ```

- Comunicação: Definir estruturas e algoritmos necessários para a comunicação. A utilização da comunicação entre as threads se da através da struct dotdata, e o mutex mutexsum, tais variáveis são globais e compartilhadas entre as threads.

  ```c
  pthread_mutex_lock (&mutexsum);
  dotdata.c += mysum;
  pthread_mutex_unlock (&mutexsum)
  ```

- Aglomeração: Reduzir comunicações. O código abaixo serve para ilustra esta etapa. Para realizar o cálculo das multiplicações é necessário 2(dois) valores, oriundos dos vetores A e B da estrutura 'dotdata_t'. O calculo das somas carece do resultado da multiplicação executada anteriormente.

  ```c
  for (k = 0; k < dotdata.repeat; k++) {
     mysum = 0.0;
     for (i = start; i < end ; i++)  {
        mysum += (a[i] * b[i]);
     }
  }

  ```

* Mapeamento: Distribuir as tarefas. Temos a função dotprod_threads() para exemplificar a distribuição das threads.

  ```c
  void dotprod_threads(int nthreads)
  {
   int i;
   pthread_t *threads;
   pthread_attr_t attr;

   threads = (pthread_t *) malloc(nthreads * sizeof(pthread_t));
   pthread_mutex_init(&mutexsum, NULL);

   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   // Aqui, a tarefa é distribuída para nthreads. A tarefa a ser
   // realizada por cada thread é descrita pela função dotprod_worker().
   for (i = 0; i < nthreads; i++) {
      pthread_create(&threads[i], &attr, dotprod_worker, (void *) i);
   }
   pthread_attr_destroy(&attr);
   for (i = 0; i < nthreads; i++) {
      pthread_join(threads[i], NULL);
   }
   free(threads);
  }
  ```

2.  Considerando o tempo mostrado na saída do programa, qual foi a aceleração com o uso de threads?

Utilizando o conjunto de dados abaixo, foi obtido o valor de, aproximadamente, .924 para a aceleração.

```
  ./pthreads_dotprod 1 1000000 1000
  ./pthreads_dotprod 2 500000 1000
```

3. A aceleração se sustenta para outros tamanhos de vetores, números de threads e repetições ?

   Para auxiliar a responder esta pergunta o programa foi executado com variados valores de conjunto de entrada, e para cada conjunto foram realizadas 20 execuções. No geral a resposta é sim, a cada execução o número de threads era aumentado, até 4 threads(o mesmo número de threads da maquina utilizada nos testes), juntamente com o valor da aceleração. Extrapolando os limites fisicos da máquina, ou seja, utilizando 8 threads, foi possível notar uma queda no valor de aceleração, sendo possível demonstrar tal limite, mantimento de mais threads do que o hardware suporta.

4. Elabore uma tabela de aceleração a partir dos dados obtidos no exercício anterior.

   Para cada conjunto de entrada foram realizadas 10 execuções.

- 1000 repetições

  - Tempo de execução
  
    | Tam. Vetor | 1Thread(s) | 2Thread(s) | 4Thread(s) |
    |:---:|:---:|:---:|:---:|
    | 100000     | 576603     | 300393     | 242927     |
    | 500000     | 3013340    | 1499717    | 1239554    |
    | 1000000    | 6007634    | 3065655    | 2485807    |
    | 2000000    | 12041939   | 6164604    | 4981710    |

  - Speedup
  
    | Tam. Vetor | 2Thread(s) | 4Thread(s) |
    | :--------: | :--------: | :--------: |
    | 100000 | 1,919 | 2,373 |
    | 500000 | 2,009 | 2,430 |
    | 1000000 | 1,959 | 2,416 |
    | 2000000 | 1,953 | 2,417 |

- 2000 repetições

  - Tempo de execução (usec)
    | Tam. Vetor | 1Thread(s) | 2Thread(s) | 4Thread(s) |
    |:----------:|:----------:|:----------:|:----------:|
    | 100000 | 1147293 | 596216 | 476966 |
    | 500000 | 5991175 | 2963642 | 2495406 |
    | 1000000 | 12069582 | 6156532 | 5064421 |
    | 2000000 | 24084261 | 12425251 | 10170673 |

  - Speedup
    | Tam. Vetor | 2Thread(s) | 4Thread(s) |
    |:----------:|:----------:|:----------:|
    | 100000 | 1,924 | 2,405 |
    | 500000 | 2,021 | 2,401 |
    | 1000000 | 1,960 | 2,383 |
    | 2000000 | 1,938 | 2,368 |

- 3000 repetições

  - Tempo de execução (usec)
    | Tam. Vetor | 1Thread(s) | 2Thread(s) | 4Thread(s) |
    |:----------:|:----------:|:----------:|:----------:|
    | 100000 | 1709646 | 947634 | 858673 |
    | 500000 | 8938235 | 4463446 | 3784286 |
    | 1000000 | 18013700 | 9264967 | 7407861 |
    | 2000000 | 41935065 | 18196366 | 14693602 |

  - Speedup
    | Tam. Vetor | 2Thread(s) | 4Thread(s) |
    |:----------:|:----------:|:----------:|
    | 100000 | 1,804 | 1,991 |
    | 500000 | 2,002 | 2,361 |
    | 1000000 | 1,944 | 2,431 |
    | 2000000 | 2,304 | 2,853 |

5. Explique as diferenças entre pthreads_dotprod.c e pthreads_dotprod2.c. Com as linhas removidas, o programa está correto?

   A diferença entre os arquivos, pthreads_dotprod.c e pthreads_dotprod2.c, é a utilização, no primeiro, de um mutex no momento em que ocorre a soma na estrutura compartilhada pelas threads. Com essa diferença o programa não executará corretamente, pois o resultado esperado será diferente do obtido, devido a falta de sincronização no acesso das threads à variavel da estrutura compartilhada.

## Parte II: OpenMP

1. Implemente um programa equivalente a [pthreads_dotprod.c](pthreads_dotprod/pthreads_dotprod.c) usando OpenMP.

   - A implementação encontra-se na pasta do openmp. [Código Completo](openmp/codeOMP.c)

2. Avalie o desempenho do programa em OpenMP, usando os mesmos dados/argumentos do programa com threads POSIX.

- Speedup - 1000 repetições
  | Tam. Vetor | 2Thread(s) | 4Thread(s) |
  |:----------:|:----------:|:----------:|
  | 100000 | 2.106 | 2.351 |
  | 500000 | 2.038 | 2.430 |
  | 1000000 | 1.967 | 2.386 |
  | 2000000 | 1.935 | 2.334 |

- Speedup - 2000 repetições
  | Tam. Vetor | 2Thread(s) | 4Thread(s) |
  |:----------:|:----------:|:----------:|
  | 100000 | 1.875 | 2.309 |
  | 500000 | 2.021 | 2.400 |
  | 1000000 | 1.960 | 2.383 |
  | 2000000 | 1.938 | 2.368 |

- Speedup - 3000 repetições
  | Tam. Vetor | 2Thread(s) | 4Thread(s) |
  |:----------:|:----------:|:----------:|
  | 100000 | 1.929 | 2.283 |
  | 500000 | 2.022 | 2.420 |
  | 1000000 | 1.951 | 2.383 |
  | 2000000 | 1.943 | 2.368 |

Os valores de speedup obtidos nas execuções utilizado threads POSIX foram superiores aos obtidos em OpenMP. Era esperado um melhor desempenho utilizando OMP, porém não foi o que ocorreu.
