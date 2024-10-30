CC=gcc
CFLAGS=-I.
# Правило для сборки исполняемого файла parallel_min_max
parallel_min_max: utils.o find_min_max.o parallel_min_max.c
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)
# Правило для сборки объекта utils.o
utils.o: utils.h
	$(CC) -c utils.c $(CFLAGS)
# Правило для сборки объекта find_min_max.o
find_min_max.o: utils.h find_min_max.h
	$(CC) -c find_min_max.c $(CFLAGS)
# Правило для сборки исполняемого файла zombi_demo
zombie_demo: zombie_demo.o
	$(CC) -o zombie_demo zombie_demo.o
# Правило для сборки объекта zombie_demo.o
zombie_demo.o: zombie_demo.c
	$(CC) -c zombie_demo.c
# Правило для сборки исполняемого файла process_memory
process_memory: process_memory.o
	$(CC) -o process_memory process_memory.o

# Правило для сборки объекта process_memory.o
process_memory.o: process_memory.c
	$(CC) -c process_memory.c
# Правило для сборки исполняемого файла parallel_sum
parallel_sum: parallel_sum.o
	$(CC) -o parallel_sum parallel_sum.o -lpthread $(CFLAGS)

# Правило для сборки объекта parallel_sum.o
parallel_sum.o: parallel_sum.c
	$(CC) -c parallel_sum.c $(CFLAGS)

# Правило для очистки временных файлов
clean:
	rm -f utils.o find_min_max.o  parallel_min_max zombie_demo.o zombie_demo process_memory.o process_memory parallel_sum.o parallel_sum
# Правило для сборки всех файлов
all:parallel_min_max zombie_demo process_memory parallel_sum