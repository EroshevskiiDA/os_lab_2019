#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  char *args[] = {"./sequential_min_max", NULL}; // Аргументы для execlp

  // Вызываем execlp для запуска sequential_min_max в отдельном процессе
  if (execlp("./sequential_min_max", args[0], NULL) == -1) {
    perror("Ошибка вызова execlp");
    exit(EXIT_FAILURE);
  }

  return 0; // Эта строка не выполняется, так как execlp заменяет текущий процесс
}