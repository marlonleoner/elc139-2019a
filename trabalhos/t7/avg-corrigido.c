// Adaptado de www.mpitutorial.com
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <assert.h>

// Cria um vetor de numeros aleatórios. Os valores gerados estão entre 0 e 1
float *create_rand_nums(int num_elements) {
    float *rand_nums = (float *)malloc(sizeof(float) * num_elements);
    assert(rand_nums != NULL);
    int i;
    for (i = 0; i < num_elements; i++) {
        rand_nums[i] = (rand() / (float)RAND_MAX);
    }
    return rand_nums;
}

// Calcula a media dos valores para um vetor
float compute_avg(float *array, int num_elements) {
    float sum = 0.f;
    int i;
    for (i = 0; i < num_elements; i++) {
        sum += array[i];
    }
    return sum / num_elements;
}

int main(int argc, char** argv) {
    
    //==== VARIAR O TAMANHO DO VETOR AQUI! ====//
    int SIZE_PER_PROC = 1000;
    //=========================================//
    
    MPI_Init(&argc, &argv);
    
    double start_time_mpi, end_time_mpi;
    int num_elements_per_proc = SIZE_PER_PROC;

    // Semente para a geração dos numeros aleatórios
    srand(time(NULL));

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Crie um vetor com valores aleatórios no processo root. 
    // O tamanho total desse vetor será o número de elementos por processo
    // vezes o número de processos

    float *rand_nums = NULL;
    if (world_rank == 0) {
        rand_nums = create_rand_nums(num_elements_per_proc * world_size);
    }

    // Para cada processo, crie um buffer contendo um subconjunto de todo o vetor
    float *sub_rand_nums = (float *)malloc(sizeof(float) * num_elements_per_proc);
    assert(sub_rand_nums != NULL);

    // Realiza um Scatter dos números aleatórios do processo root para todos os demais processos.
    MPI_Scatter(rand_nums, num_elements_per_proc, MPI_FLOAT, sub_rand_nums,
                num_elements_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Calcule a média do subconjunto
    float sub_avg = compute_avg(sub_rand_nums, num_elements_per_proc);

    // Realizar um Gather de todas as médias parciais para o processo root
    float *sub_avgs = NULL;
    if (world_rank == 0) {
        sub_avgs = (float *)malloc(sizeof(float) * world_size);
        assert(sub_avgs != NULL);
    }

    MPI_Gather(&sub_avg, 1, MPI_FLOAT, sub_avgs, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Agora que temos todas as médias parciais no processo root, calcule a média total de
    // todos os números. Como estamos assumindo que cada processo calculou uma média
    // em uma quantidade igual de elementos, esse cálculo produzirá a resposta correta.

    if (world_rank == 0) {

        float avg = compute_avg(sub_avgs, world_size);
        printf("Avg of all elements is %f\n", avg);

        // Calcule a média entre os dados originais para comparação
        float original_data_avg = compute_avg(rand_nums, num_elements_per_proc * world_size);
        printf("Avg computed across original data is %f\n", original_data_avg);
    }

    // Free nos vetores
    if (world_rank == 0) {
        free(rand_nums);
        free(sub_avgs);
    }
    free(sub_rand_nums);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}
