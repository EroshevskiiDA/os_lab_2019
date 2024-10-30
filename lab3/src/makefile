CC=gcc
CFLAGS=-I.

# Правило для сборки исполняемого файла sequential_min_max
sequential_min_max: utils.o find_min_max.o sequential_min_max.c
	$(CC) -o sequential_min_max find_min_max.o utils.o sequential_min_max.c $(CFLAGS)

# Правило для сборки исполняемого файла parallel_min_max
parallel_min_max: utils.o find_min_max.o parallel_min_max.c
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

# Правило для сборки исполняемого файла exec_sequential
exec_sequential: exec_sequential.o sequential_min_max
	$(CC) -o exec_sequential exec_sequential.o -L. sequential_min_max

# Правило для сборки объекта utils.o
utils.o: utils.h
	$(CC) -c utils.c $(CFLAGS)

# Правило для сборки объекта find_min_max.o
find_min_max.o: utils.h find_min_max.h
	$(CC) -c find_min_max.c $(CFLAGS)

# Правило для сборки объекта exec_sequential.o
exec_sequential.o: exec_sequential.c
	$(CC) -c exec_sequential.c $(CFLAGS)

# Правило для очистки временных файлов
clean:
	rm -f utils.o find_min_max.o sequential_min_max parallel_min_max exec_sequential exec_sequential.o 

# Правило для сборки всех файлов
all: sequential_min_max parallel_min_max exec_sequential