#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#include "shmem.h"

int main(void)
{
    char *empty_string = malloc(sizeof(char) * map_size); // создание пустой строки которая по длине как общая память
    char *filename;
    int fd = shm_open(BackingFile1, O_RDWR | O_CREAT, AccessPerms);// получение файлового дискриптора
    char temp;
    int val;// зачеие симафора
    int i = 0;
    filename = (char *)malloc(sizeof(char));
    printf("Enter file name: ");
    while (temp != '\n')
    {
        scanf("%c", &temp);
        i++;
        filename = (char *)realloc(filename, i * sizeof(char));
        filename[i - 1] = temp;
    }
    filename[i - 1] = '\0';
    int file = open(filename, O_RDONLY);
    if (file == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    ftruncate(fd, map_size); // расширение орбщей памяти до нужного размера
    caddr_t memptr = mmap( // проецирование общей памяти
        NULL,
        map_size,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        0);

    if (memptr == MAP_FAILED) // проверка на ошибку
    {
        perror("mmap1");
        exit(EXIT_FAILURE);
    }
    memset(memptr, '\0', map_size); // заполнение памяти символами/0
    pid_t pid = fork(); // создание дочернего процесса
    if (pid == 0)
    {
        munmap(memptr, map_size); // подтверждение изменений 
        close(fd); // закрытие общей памяти 
        if (dup2(file, fileno(stdin)) == -1) // меняем поток ввода и проверяем на ошибки
        {
            perror("dup2 stdin");
            exit(EXIT_FAILURE);
        }
        execl("child.out", "child.out", NULL); // запуск дочерней программы
    }
    i = 0;
    close(file);
    if (waitpid(pid, NULL, 0) < 0) // ожидание завершения дочернего процесса и проверка на ошибки
    {
        fprintf(stderr, "Error waiting for child processes: %s\n", strerror(errno));
        return -1;
    }
    char *result = (char*)malloc(sizeof(char)*1024); // создание пустой строки для вывода
    strcpy(result,memptr); // копирование того, что хранится в общей памяти
    printf("%s", result);
    return 0;
}
