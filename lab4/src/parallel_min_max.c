
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <getopt.h>
#include <errno.h>
#include "find_min_max.h"
#include "utils.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <signal.h>

int main(int argc, char **argv) {
    int seed = -1;
    int array_size = -1;
    int pnum = -1;
    bool with_files = false;
    int timeout = -1; // Переменная для таймаута

    while (true) {
        int current_optind = optind ? optind : 1;
        static struct option options[] = {
            {"seed", required_argument, 0, 0},
            {"array_size", required_argument, 0, 0},
            {"pnum", required_argument, 0, 0},
            {"by_files", no_argument, 0, 'f'},
            {"timeout", required_argument, 0, 0}, // Добавление опции таймаута
            {0, 0, 0, 0}
        };
        int option_index = 0;
        int c = getopt_long(argc, argv, "f", options, &option_index);

        if (c == -1) break;

        switch (c) {
            case 0:
                switch (option_index) {
                    case 0:
                        seed = atoi(optarg);
                        break;
                    case 1:
                        array_size = atoi(optarg);
                        break;
                    case 2:
                        pnum = atoi(optarg);
                        break;
                    case 3:
                        with_files = true;
                        break;
                    case 4:
                        timeout = atoi(optarg); // Считывание таймаута
                        break;
                    default:
                        printf("Index %d is out of options\n", option_index);
                }
                break;
            case 'f':
                with_files = true;
                break;
            case '?':
                break;
            default:
                printf("getopt returned character code 0%o?\n", c);
        }
    }

    if (optind < argc) {
        printf("Has at least one no option argument\n");
        return 1;
    }

    if (seed == -1 || array_size == -1 || pnum == -1) {
        printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" [--timeout \"num\"]\n", argv[0]);
        return 1;
    }

    int *array = malloc(sizeof(int) * array_size);
    GenerateArray(array, array_size, seed);
    int active_child_processes = 0;

    struct timeval start_time;
    gettimeofday(&start_time, NULL);

    int pipe_fds[2];
    if (!with_files) {
        pipe(pipe_fds); // Создание канала
    }

    for (int i = 0; i < pnum; i++) {
        pid_t child_pid = fork();
        if (child_pid >= 0) {
            active_child_processes += 1;
            if (child_pid == 0) {
                // Дочерний процесс

                int local_min = INT_MAX;
                int local_max = INT_MIN;
                int portion_size = array_size / pnum;
                int start_index = i * portion_size;
                int end_index = (i == pnum - 1) ? array_size : (i + 1) * portion_size;

                for (int j = start_index; j < end_index; j++) {
                    if (array[j] < local_min) {
                        local_min = array[j];
                    }
                    if (array[j] > local_max) {
                        local_max = array[j];
                    }
                }

                if (with_files) {
                    // Использование файлов для записи результатов
                    char filename[20];
                    sprintf(filename, "result_%d.txt", i);
                    FILE *file = fopen(filename, "w");
                    if (file) {
                        fprintf(file, "%d %d\n", local_min, local_max);
                        fclose(file);
                    }
                } else {
                    // Использование канала для записи результатов
                    write(pipe_fds[1], &local_min, sizeof(int));
                    write(pipe_fds[1], &local_max, sizeof(int));
                }
                exit(0); // Завершение дочернего процесса
            }
        } else {
            printf("Fork failed!\n");
            return 1;
        }
    }

    if (timeout != -1) {
        // Установка сигнала SIGKILL для таймаута
        signal(SIGALRM, SIG_DFL); // Возврат к стандартной обработке SIGALRM

        // Настройка таймера
        struct itimerval timer;
        timer.it_value.tv_sec = timeout;
        timer.it_value.tv_usec = 0;
        timer.it_interval.tv_sec = 0;
        timer.it_interval.tv_usec = 0;
        setitimer(ITIMER_REAL, &timer, NULL);

        // Ожидание завершения дочерних процессов или срабатывания таймера
        while (active_child_processes > 0) {
            pid_t pid = wait(NULL);
            if (pid > 0) {
                active_child_processes--;
            } else if (pid == -1) {
                if (errno == EINTR) {
                    // Срабатывание таймера
                    printf("Таймаут: прерывание процессов.\n");
                    for (int i = 0; i < pnum; i++) {
                        kill(i, SIGKILL); // Отправка SIGKILL всем дочерним процессам
                    }
                    active_child_processes = 0;
                } else {
                    perror("Ошибка ожидания завершения процессов");
                    return 1;
                }
            }
        }

        // Отключение таймера
        timer.it_value.tv_sec = 0;
        timer.it_value.tv_usec = 0;
        setitimer(ITIMER_REAL, &timer, NULL);
    } else {
        // Ожидание завершения дочерних процессов без таймаута
        while (active_child_processes > 0) {
            pid_t pid = wait(NULL);
            if (pid > 0) {
                active_child_processes--;
            } else if (pid == -1) {
                perror("Ошибка ожидания завершения процессов");
                return 1;
            }
        }
    }

    if (!with_files) {
        // Чтение результатов из канала
        int global_min, global_max;
        read(pipe_fds[0], &global_min, sizeof(int));
        read(pipe_fds[0], &global_max, sizeof(int));
        printf("Минимальное значение: %d\n", global_min);
        printf("Максимальное значение: %d\n", global_max);
        close(pipe_fds[0]);
        close(pipe_fds[1]);
    }

    free(array);
    return 0;
}
