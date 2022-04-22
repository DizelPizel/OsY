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
    map_fd = shm_open(BackingFile1, O_RDWR, AccessPerms); // открытие общей области памяти
    if (map_fd < 0)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    caddr_t memptr = mmap( // проецируем память к себе
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
            sprintf(memptr, "%s", delimoe); // печатаем в область памяти
        }
    return 0;
}
