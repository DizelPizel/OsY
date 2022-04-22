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
    char *empty_string = malloc(sizeof(char) * map_size);
    char *filename;
    int fd = shm_open(BackingFile1, O_RDWR | O_CREAT, AccessPerms);
    char temp;
    int val;
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
    ftruncate(fd, map_size);
    caddr_t memptr = mmap(
        NULL,
        map_size,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        0);

    if (memptr == MAP_FAILED)
    {
        perror("mmap1");
        exit(EXIT_FAILURE);
    }
    memset(memptr, '\0', map_size);
    pid_t pid = fork();
    if (pid == 0)
    {
        munmap(memptr, map_size);
        close(fd);
        if (dup2(file, fileno(stdin)) == -1)
        {
            perror("dup2 stdin");
            exit(EXIT_FAILURE);
        }
        execl("child.out", "child.out", NULL);
    }
    i = 0;
    close(file);
    char *str = (char *)malloc(sizeof(char));
    if (waitpid(pid, NULL, 0) < 0)
    {
        fprintf(stderr, "Error waiting for child processes: %s\n", strerror(errno));
        return -1;
    }
    char *result = (char*)malloc(sizeof(char)*1024);
    strcpy(result,memptr);
    printf("%s", result);
    return 0;
}
