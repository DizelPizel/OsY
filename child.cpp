#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>
#include "shmem.h"

int main(void)
{
    std::string tmp;
    int i = 0;
    int map_fd;
    sem_t *semptr;
    map_fd = shm_open(BackingFile1, O_RDWR, AccessPerms);
    semptr = sem_open(SemaphoreName1, O_CREAT, AccessPerms, 2);
    if (map_fd < 0)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    caddr_t memptr = mmap(
        NULL,
        map_size,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        map_fd,
        0);
    if (memptr == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    if (semptr == SEM_FAILED)
    {
        perror("semptr");
        exit(EXIT_FAILURE);
    }

    if (sem_wait(semptr) != 0)
    {
        perror("sem_wait");
        exit(EXIT_FAILURE);
    }
    int val;
    while (memptr[0] != EOF)
    {
        if (sem_getvalue(semptr, &val) != 0)
        {
            perror("sem_getvalue");
            exit(EXIT_FAILURE);
        }
        if (val == 2)
        {
            continue;
        }
        if (sem_wait(semptr) != 0)
        {
            perror("sem_wait");
            exit(EXIT_FAILURE);
        }
        if (memptr[0] == EOF)
        {
            break;
        }
        if (memptr[0] == '\0')
        {
            if (sem_post(semptr) != 0)
            {
                perror("sem_post");
                exit(EXIT_FAILURE);
            }
            continue;
        }
        while (std::getline(std::cin, tmp))
        {
            float delimoe;
            int delitel;
            std::stringstream ss(tmp);
            ss >> delimoe;
            i++;
            while (ss >> delitel)
            {
                if (!delitel)
                {
                    return -1;
                }
                delimoe /= delitel;
            }
            if (sem_post(semptr) != 0)
            {
                perror("sem_post");
                exit(EXIT_FAILURE);
            }
            printf("%s", delimoe);
            sprintf(memptr, "%s", delimoe);
        }
    }
    return 0;
}
