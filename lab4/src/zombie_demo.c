#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // Дочерний процесс
        printf("Дочерний процесс с PID = %d завершает работу.\n", getpid());
        exit(0);  // Завершение дочернего процесса
    } else if (pid > 0) {
        // Родительский процесс
        printf("Родительский процесс с PID = %d. Создан зомби-процесс.\n", getpid());
        sleep(10);  // Задержка, позволяющая увидеть зомби-процесс
        printf("Родитель вызывает wait() для завершения дочернего процесса.\n");
        wait(NULL);  // Сбор статуса дочернего процесса
        printf("Дочерний процесс собран.\n");
    } else {
        perror("Ошибка fork");
        return 1;
    }

    return 0;
}