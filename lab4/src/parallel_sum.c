#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

struct SumArgs {
    int *array;
    int begin;
    int end;
};

int Sum(const struct SumArgs *args) {
    int sum = 0;
    for (int i = args->begin; i < args->end; i++) {
        sum += args->array[i];
    }
    return sum;
}

void *ThreadSum(void *args) {
    struct SumArgs *sum_args = (struct SumArgs *)args;
    return (void *)(size_t)Sum(sum_args);
}

void GenerateArray(int *array, size_t size, unsigned int seed) {
    srand(seed);
    for (size_t i = 0; i < size; i++) {
        array[i] = rand() % 100; // Генерация случайных чисел от 0 до 99
    }
}

int main(int argc, char **argv) {
    if (argc != 7) {
        printf("Usage: %s --threads_num <num> --seed <num> --array_size <num>n", argv[0]);
        return 1;
    }

    uint32_t threads_num = atoi(argv[2]);
    uint32_t array_size = atoi(argv[4]);
    uint32_t seed = atoi(argv[6]);

    pthread_t threads[threads_num];
    int *array = malloc(sizeof(int) * array_size);
    
    // Генерация массива
    GenerateArray(array, array_size, seed);

    struct SumArgs args[threads_num];
    int chunk_size = array_size / threads_num;

    // Измерение времени
    clock_t start_time = clock();

    for (uint32_t i = 0; i < threads_num; i++) {
        args[i].array = array;
        args[i].begin = i * chunk_size;
        args[i].end = (i == threads_num - 1) ? array_size : (i + 1) * chunk_size; // Обработка остатка в последнем потоке

        if (pthread_create(&threads[i], NULL, ThreadSum, (void *)&args[i])) {
            printf("Error: pthread_create failed!n");
            free(array);
            return 1;
        }
    }

    int total_sum = 0;
    for (uint32_t i = 0; i < threads_num; i++) {
        int sum = 0;
        pthread_join(threads[i], (void **)&sum);
        total_sum += sum;
    }

    clock_t end_time = clock();
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    free(array);
    printf("Total: %dn", total_sum);
    printf("Time taken: %.6f secondsn", time_taken);
    
    return 0;
}