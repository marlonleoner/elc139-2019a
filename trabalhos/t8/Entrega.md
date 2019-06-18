# T8: Geração de Imagem em Paralelo com CUDA

## Identificação

Nome: Marlon Leoner da Silva Rodrigues<br/>Disciplina: [Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2019a)

## Link dos programas

+ wavecuda1.cu: [código](wavecuda1.cu)
+ wavecuda2.cu: [código](wavecuda2.cu)

## Parte 1

+ Implemente um programa nomeado [wavecuda1.cu](wavecuda1.cu), em que cada frame seja computado em paralelo por uma thread diferente.

   Para que cada frame fosse computado em paralelo por uma thread diferente era necessário utilizar o código abaixo, utilizando o número de frames no parâmetro que define o número de threads.
   ```c
   fPixelGenerator<<<1, frames>>>(width, pic);
   ```

+ Analise o desempenho do programa, começando pela medição dos tempos de execução sequencial e paralela para pelo menos 4 diferentes entradas do programa, sendo: a) 1024 100, b) 1024 200 e outras 2 entradas à sua escolha. Compare os tempos obtidos. Use nvprof para analisar o perfil de execução em GPU. Explique seus resultados.

   ```c
   !nvprof ./wavecuda1 1024 100

   computing 100 frames of 1024 by 1024 picture
   ==6867== NVPROF is profiling process 6867, command: ./wavecuda1 1024 100
   compute time: 0.6599 s
   ==6867== Profiling application: ./wavecuda1 1024 100
   ==6867== Profiling result:
               Type  Time(%)      Time     Calls       Avg       Min       Max  Name
   GPU activities: 100.00%  659.77ms         1  659.77ms  659.77ms  659.77ms  fPixelGenerator(int, unsigned char*)
         API calls: 71.57%  659.79ms         1  659.79ms  659.79ms  659.79ms  cudaDeviceSynchronize
                    28.05%  258.56ms         1  258.56ms  258.56ms  258.56ms  cudaMallocManaged
                     0.33%  3.0467ms         1  3.0467ms  3.0467ms  3.0467ms  cudaFree
                     0.02%  189.52us         1  189.52us  189.52us  189.52us  cuDeviceTotalMem
                     0.02%  154.82us        96  1.6120us     141ns  65.777us  cuDeviceGetAttribute
                     0.01%  65.767us         1  65.767us  65.767us  65.767us  cudaLaunchKernel
                     0.00%  29.388us         1  29.388us  29.388us  29.388us  cuDeviceGetName
                     0.00%  2.8470us         1  2.8470us  2.8470us  2.8470us  cuDeviceGetPCIBusId
                     0.00%  2.3280us         3     776ns     157ns  1.7580us  cuDeviceGetCount
                     0.00%  1.4960us         2     748ns     246ns  1.2500us  cuDeviceGet
                     0.00%     275ns         1     275ns     275ns     275ns  cuDeviceGetUuid

   ==6867== Unified Memory profiling result:
   Device "Tesla T4 (0)"
   Count  Avg Size  Min Size  Max Size  Total Size  Total Time  Name
      52         -         -         -           -  9.907296ms  Gpu page fault groups
   ```


## Parte 2

+ Implemente um programa nomeado [wavecuda2.cu](wavecuda2.cu), que aproveite melhor o paralelismo da GPU.

   Para esta segunda parte foi utilizado múltiplos blocos para fosse possível aproveitar melhor o paralelismo da GPU. Os frames foram divididos entre os blocos, e as rows do frame foram dividas entre as threads do bloco.
   ```c
   fPixelGenerator<<<blocks, threads>>>(width, frames, pic);
   ```

+ Desempenho.

   ```c
   computing 100 frames of 1024 by 1024 picture - 4 blocks w/ 128 threads.
   ==6924== NVPROF is profiling process 6924, command: ./wavecuda2 1024 100 4 128
   compute time: 0.2575 s
   ==6924== Profiling application: ./wavecuda2 1024 100 4 128
   ==6924== Profiling result:
               Type  Time(%)      Time     Calls       Avg       Min       Max  Name
   GPU activities: 100.00%  257.32ms         1  257.32ms  257.32ms  257.32ms  fPixelGenerator(int, int, unsigned char*)
         API calls: 49.97%  257.41ms         1  257.41ms  257.41ms  257.41ms  cudaDeviceSynchronize
                    49.32%  254.06ms         1  254.06ms  254.06ms  254.06ms  cudaMallocManaged
                     0.63%  3.2314ms         1  3.2314ms  3.2314ms  3.2314ms  cudaFree
                     0.04%  193.69us         1  193.69us  193.69us  193.69us  cuDeviceTotalMem
                     0.03%  153.47us        96  1.5980us     140ns  65.272us  cuDeviceGetAttribute
                     0.01%  41.845us         1  41.845us  41.845us  41.845us  cudaLaunchKernel
                     0.01%  40.294us         1  40.294us  40.294us  40.294us  cuDeviceGetName
                     0.00%  2.9240us         1  2.9240us  2.9240us  2.9240us  cuDeviceGetPCIBusId
                     0.00%  2.3420us         3     780ns     196ns  1.7220us  cuDeviceGetCount
                     0.00%  1.2200us         2     610ns     328ns     892ns  cuDeviceGet
                     0.00%     297ns         1     297ns     297ns     297ns  cuDeviceGetUuid

   ==6924== Unified Memory profiling result:
   Device "Tesla T4 (0)"
      Count  Avg Size  Min Size  Max Size  Total Size  Total Time  Name
      110         -         -         -           -  15.81299ms  Gpu page fault groups
   ```


### Apresentação

+ [Slides](slides.pdf)