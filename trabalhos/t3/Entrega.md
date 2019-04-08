# T3: Scheduling com OpenMP

## Identificação

Nome: Marlon Leoner da Silva Rodrigues
Disciplina: [Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2019a)

## Link do Programa

Link do programa desenvolvido: [OpenMPDemoABC.cpp](OpenMPDemoABC.cpp)

## Execuções
Para os resultados demonstrados abaixo, foi utilizado o seguinte conjunto de valores:
   - Número de THREADS: 4
   - Tamanho ARRAY: 60
   - Valor do CHUNK: 10

```
NThreads: 4 - NTotal: 60 - NChunk: 10

===========================
===   Cases With Lock   ===
===========================
Case 1: Static
ABDDDDDDDDDDDDDDDBBBBBBBBBBBBBBAAAAAAAAAAAAAACCCCCCCCCCCCCCC
A=15 B=15 C=15 D=15 

Case 2: Static && Chunk
CAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBDDDDDDDDDDCCCCCCCCC
A=20 B=20 C=10 D=10 

Case 3: Dynamic
BAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACBD
A=56 B=2 C=1 D=1 

Case 4: Dynamic && Chunk
DCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDAAAAAAAAAABBBBBBBBBB
A=10 B=10 C=30 D=10 

Case 5: Guided
BDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDBBBBBBBBBBBBBBAAAAAAAAACCCCCC
A=9 B=15 C=6 D=30 

Case 6: Guided && Chunk
CBAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBDDDDDDDDDDCCCCCCCCCCCCCC
A=23 B=12 C=15 D=10 

Case 7: Runtime
CDDDCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCBDA
A=1 B=1 C=54 D=4 

Case 8: Auto
BCCCCCCCCCCCCCCCBBBBBBBBBBBBBBAAAAAAAAAAAAAAADDDDDDDDDDDDDDD
A=15 B=15 C=15 D=15 

===========================
=== Cases Without Lock  ===
===========================
Case 1: Static
CBADCBADABDCBACDBADBADBACDBACBADBACDBACDBACDBADCAB-DC-DC-CCC
A=14 B=14 C=15 D=14 

Case 2: Static && Chunk
ABADCABDCBADBACDBADCBADBCABDACBDACBADCBA-BCA-BABABABABABABA-
A=20 B=19 C=9 D=9 

Case 3: Dynamic
DCABDCABACDBACDBACBACDBACDACBADCBACDBACDBACDABCDACBDACBAC---
A=16 B=13 C=16 D=12 

Case 4: Dynamic && Chunk
DCABDACBACDBACDBACBACDACBDACBDACBADCBACD-ACDAC-ACACACACACAC-
A=19 B=9 C=19 D=10 

Case 5: Guided
AADCBADCBADCADCACDACDABCDACADCDABCDABCDAADCADBCADBCDACDBA---
A=18 B=8 C=15 D=16 

Case 6: Guided && Chunk
ADABCDABDCABDACBDACDBACDABDACBDACBDACBDACDABDCABDAC--ACA-AAA
A=20 B=11 C=12 D=14 

Case 7: Runtime
BDACBDACBDAACBDADCBADACBDACBDADCBADCBADCBADACBDADCBADCBAD---
A=16 B=13 C=12 D=16 

Case 8: Auto
BDACBADCADBCADBACDADBCADBCADBACDABDCADBCADBACDBACD-B-CBCBCB-
A=14 B=15 C=14 D=14
```

## Referência

- [Introdução ao OpenMP](https://www.dcc.fc.up.pt/~fds/aulas/PPD/0708/intro_openmp-1x2.pdf)
  Introdução ao OpenMP by Fernando Silva(DCC - FCUP)
- [OpenMP: For & Scheduling](http://jakascorner.com/blog/2016/06/omp-for-scheduling.html)
  OpenMP: For & Scheduling by Jaka's Corner
- [OpenMP Scheduling](http://www.inf.ufsc.br/~bosco.sobral/ensino/ine5645/OpenMP_Dynamic_Scheduling.pdf)

